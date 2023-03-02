#include "db.h"
std::shared_mutex smutex_;
ThreadPool threadpool(THREAD_POOL_NUM);
// Although sizeof can be obtained at compile time
static constexpr uint32_t fixedEntryHeadLen = 3 * sizeof(uint32_t);
static constexpr uint32_t sizeofUint32 = sizeof(uint32_t);
static constexpr uint32_t sizeofChar = sizeof(char);

MiniKVDB::MiniKVDB() {
    hashSize_ = HASH_SIZE_INIT;
    hash1_ = std::shared_ptr<HashTable>(new HashTable(hashSize_));
    hash2_ = std::shared_ptr<HashTable>(new HashTable(hashSize_));
    expires_ = std::unique_ptr<HashTable>(new HashTable(hashSize_));
    io_ = std::unique_ptr<KVio>(new KVio(RDB_FILE_NAME));
    rdbFileReadInitDB();
    rdbe_ = new rdbEntry();
    timer_ = std::shared_ptr<KVTimer>(new KVTimer());
    timer_->start(RDB_INTERVAL, std::bind(&MiniKVDB::rdbSave, this));
    timer_->start(REHASH_DETECT_INTERVAL, std::bind(&MiniKVDB::rehash, this));
    rehashFlag_ = false;
}

MiniKVDB::~MiniKVDB() {
    timer_->stop();
    io_->flushCache();
    delete rdbe_;
}

void MiniKVDB::insert(std::string key, std::string val, uint32_t encoding) {
    if (!rehashFlag_) {
        hash1_->insert(key, val, encoding);
        return;
    }
    if (hash1_->exist(key)) {
        hash1_->insert(key, val, encoding);
        return;
    }
    hash2_->insert(key, val, encoding);
    progressiveRehash(hash2_);
}

bool MiniKVDB::expired(std::string key) {
    std::vector<std::string> e;
    expires_->get(key, e);
    if (!e.empty()) {
        uint64_t expires = stoi(e[0]);
        auto now = std::chrono::system_clock::now(); 
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        if (timestamp >= expires) {
            return true;
        }
    }
    return false;
}

void MiniKVDB::get(std::string key, std::vector<std::string>& res) {
    // there are 2 cases for empty res:
    // 1. exceed the time limit
    // 2. no data
    if (expired(key)) {
        res = std::vector<std::string>();
        hash1_->del(key); // lazy delete mode
        expires_->del(key);
        return;
    }
    if (!rehashFlag_) {
        hash1_->get(key, res);
        return;
    }
    if (hash1_->exist(key)) {
        hash1_->get(key, res);
        return;
    }
    hash2_->get(key, res);
    progressiveRehash(hash2_);
}

int MiniKVDB::del(std::string key) {
    return hash1_->del(key);
}

void MiniKVDB::getKeyName(std::string keyRex, std::vector<std::string>& res) {
    if (!rehashFlag_) {
        hash1_->findKeyName(keyRex, res);
        return;
    }
    hash2_->findKeyName(keyRex, res);
}

int MiniKVDB::setExpire(std::string key, uint64_t expires) {
    bool exist = hash1_->exist(key);
    if (!exist) return MiniKV_SET_EXPIRE_FAIL;
    expires_->insert(key, to_string(expires), MiniKV_STRING);
    return MiniKV_SET_EXPIRE_SUCCESS;
}

void MiniKVDB::rdbSave() {
    if (rehashFlag_) return;
    // TODO: save the expire time
    std::unique_lock<std::shared_mutex> lk(smutex_);
    kvlogi("rdb save triggered!");
    io_->clearFile();
    for (int i = 0; i < hash1_->hash_.size(); ++i) {
        auto d = hash1_->hash_[i];
        if (d.empty()) {
            continue;
        }
        for (auto it = d.begin(); it != d.end(); ++it) {
            std::string key = it->get()->key;
            if (expired(key)) continue;
            uint32_t encoding = it->get()->encoding;
            void* data = it->get()->data.get();
            saveKVWithEncoding(key, encoding, data);
        }
    }
    io_->flushCache(); // C cache->Page cache
}

void MiniKVDB::saveKVWithEncoding(std::string key, uint32_t encoding, void* data) {
    switch (encoding) {
        case MiniKV_STRING : {
            makeStringObject2RDBEntry(rdbe_, key, encoding, data);
            break;
        }
        case MiniKV_LIST : {
            makeListObject2RDBEntry(rdbe_, key, encoding, data);
            break;
        }
    }
    rdbEntryWrite(rdbe_);
}

void MiniKVDB::makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {
    uint32_t keyLen = (uint32_t)key.size();
    kvString* kvStr = (kvString*)data;
    uint32_t totalLen = fixedEntryHeadLen + keyLen + sizeofUint32 + kvStr->len;
    rdbe->totalLen = totalLen;
    rdbe->encoding = encoding;
    rdbe->keyLen = keyLen;

    char* buf = new char[keyLen]; 
    memcpy(buf, key.data(), keyLen);
    rdbe->key = std::shared_ptr<char[]>(buf);

    buf = new char[sizeofUint32 + strlen(kvStr->data.get())];
    memcpy(buf, kvStr, sizeofUint32);
    memcpy(buf + sizeofUint32, kvStr->data.get(), kvStr->len);
    rdbe->data = std::shared_ptr<char[]>(buf);
}

void MiniKVDB::makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {
    uint32_t keyLen = (uint32_t)key.size();
    auto lst = (std::list<kvString>*)data;
    uint32_t totalLen = fixedEntryHeadLen + keyLen;
    int bytes = 0;
    for (auto it = lst->begin(); it != lst->end(); ++it) {
        bytes += sizeofUint32 + it->len;
    }
    char* buf = new char[bytes];
    char* cur = buf;
    for (auto it = lst->begin(); it != lst->end(); ++it) {
        memcpy(cur, (char*)(&(*it)), sizeofUint32);
        cur += sizeofUint32;
        memcpy(cur, it->data.get(), it->len);
        cur += it->len; 
        totalLen += sizeofUint32 + it->len;
    }
    rdbe->data = std::shared_ptr<char[]>(buf);
    rdbe->totalLen = totalLen;
    rdbe->encoding = encoding;
    rdbe->keyLen = keyLen;
    buf = new char[keyLen]; 
    memcpy(buf, key.data(), keyLen);
    rdbe->key = std::shared_ptr<char[]>(buf);
}

void MiniKVDB::rdbEntryWrite(rdbEntry* rdbe) {
    int rest = io_->kvioFileWrite(rdbe, fixedEntryHeadLen); // write totalLen, encoding and keyLen
    CHECK_REST_AND_LOG(rest);
    size_t keyLen = rdbe->keyLen;
    size_t dataLen = rdbe->totalLen - fixedEntryHeadLen - keyLen;
    rest = io_->kvioFileWrite(rdbe->key.get(), keyLen);
    CHECK_REST_AND_LOG(rest);
    rest = io_->kvioFileWrite(rdbe->data.get(), dataLen);
    CHECK_REST_AND_LOG(rest);
}

void MiniKVDB::rdbLoadEntry() {
    char fixedBuf[fixedEntryHeadLen];
    int rest = io_->kvioFileRead(fixedBuf, fixedEntryHeadLen);
    CHECK_REST_AND_LOG(rest);
    int totalLen, encoding, keyLen;
    memcpy(&totalLen, fixedBuf, sizeofUint32);
    memcpy(&encoding, fixedBuf + sizeofUint32, sizeofUint32);
    memcpy(&keyLen, fixedBuf + 2 * sizeofUint32, sizeofUint32);
    
    char keyBuf[keyLen];
    rest = io_->kvioFileRead(keyBuf, keyLen);
    CHECK_REST_AND_LOG(rest);
    std::string key(keyBuf, keyBuf + keyLen);
    int waitProcessLen = totalLen - fixedEntryHeadLen - keyLen;
    uint32_t len = 0;
    switch (encoding) {
        case MiniKV_STRING : {
            rest = io_->kvioFileRead(&len, sizeofUint32);
            CHECK_REST_AND_LOG(rest);
            std::string valStr(len, 0);
            rest = io_->kvioFileRead(valStr.data(), len);
            CHECK_REST_AND_LOG(rest);
            hash1_->insert(key, valStr, encoding);
            break;
        }
        case MiniKV_LIST : {
            while (waitProcessLen > 0) {
                rest = io_->kvioFileRead(&len, sizeofUint32);
                CHECK_REST_AND_LOG(rest);
                std::string valStr(len, 0);
                rest = io_->kvioFileRead(valStr.data(), len);
                CHECK_REST_AND_LOG(rest);
                hash1_->insert(key, valStr, encoding);
                waitProcessLen -= sizeofUint32 + len;
            }
            break;
        }
    }
}

void MiniKVDB::rdbFileReadInitDB() {
    if (io_->empty()) {
        kvlogi("No rdb file to load.");
        return;
    }
    while (!io_->empty()) {
        rdbLoadEntry();
    }
    kvlogi("Rdb file loaded.");
}

void MiniKVDB::rehash() {
    if (rehashFlag_) return;
    std::shared_lock<std::shared_mutex> lk(smutex_);
    if (!hash1_->needRehash()) return;
    lk.unlock();
    rehashFlag_ = true;
    hash2_->resize(hash1_->size() * 2);
}

void MiniKVDB::progressiveRehash(std::shared_ptr<HashTable> h2) {
    if (!rehashFlag_) return;
    bool completed = hash1_->progressiveRehash(h2, REHASH_MAX_EMPTY_VISITS);
    if (completed) {
        rehashFlag_ = false;
        std::swap(hash1_, hash2_);
        hash2_->clear();
    }
}