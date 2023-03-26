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

    timerRdb_ = std::shared_ptr<KVTimer>(new KVTimer());
    timerRehash_ = std::shared_ptr<KVTimer>(new KVTimer());
    timerFixedTimeDelKey_ = std::shared_ptr<KVTimer>(new KVTimer());
    timerMemoryDetect_ = std::shared_ptr<KVTimer>(new KVTimer());

    timerRdb_->start(RDB_INTERVAL, std::bind(&MiniKVDB::rdbSave, this));
    timerRehash_->start(REHASH_DETECT_INTERVAL, std::bind(&MiniKVDB::rehash, this));
    timerFixedTimeDelKey_->start(FIXED_TIME_DELETE_EXPIRED_KEY, std::bind(&MiniKVDB::fixedTimeDeleteExpiredKey, this));
    timerMemoryDetect_->start(MEMORY_DETECT_INTERVAL, std::bind(&MiniKVDB::memoryDetector, this));
    rehashFlag_ = false;
}

MiniKVDB::~MiniKVDB() {
    timerRdb_->stop();
    timerRehash_->stop();
    timerFixedTimeDelKey_->stop();
    timerMemoryDetect_->stop();
    io_->flushCache();
    delete rdbe_;
}

void MiniKVDB::insert(std::string key, std::string val, uint32_t encoding) {
    if (!rehashFlag_) {
        std::unique_lock<std::shared_mutex> lk(smutex_);
        hash1_->insert(key, val, encoding);
        expires_->update(key);
        return;
    }
    if (hash1_->exist(key)) {
        std::unique_lock<std::shared_mutex> lk(smutex_);
        hash1_->insert(key, val, encoding);
        expires_->update(key);
        return;
    }
    std::unique_lock<std::shared_mutex> lk(smutex_);
    hash2_->insert(key, val, encoding);
    expires_->update(key);
    progressiveRehash(hash2_);
}

void MiniKVDB::fixedTimeDeleteExpiredKey() {
    std::string keyRandom;
    while (true) {
        int expiredKeyNum = 0;
        for (int i = 0; i < ACTIVE_EXPIRE_CYCLE_LOOKUPS_PER_LOOP; ++i) {
            if (expires_->keyNum() <= 0) goto breakLoop;
            keyRandom = expires_->randomKeyFind();
            if (expired(keyRandom)) {
                std::unique_lock<std::shared_mutex> lk(smutex_);
                hash1_->del(keyRandom);
                expires_->del(keyRandom);
                lk.unlock();
                expiredKeyNum++;
            }
        }
        if (static_cast<double>(expiredKeyNum) / ACTIVE_EXPIRE_CYCLE_LOOKUPS_PER_LOOP < 0.25) {
            goto breakLoop;
        }
    }
    breakLoop:
    return;
}

bool MiniKVDB::expired(std::string key) {
    std::vector<std::string> e;
    expires_->get(key, e);
    if (!e.empty()) {
        uint64_t expires = stol(e[0]); // use stoi may throw out of range error
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
    std::unique_lock<std::shared_mutex> lkS(smutex_);
    if (expired(key)) {
        res = std::vector<std::string>();
        hash1_->del(key); // lazy delete mode
        expires_->del(key);
        lkS.unlock();
        return;
    }
    if (!rehashFlag_) {
        hash1_->get(key, res);
        expires_->update(key);
        lkS.unlock();
        return;
    }
    if (hash1_->exist(key)) {
        hash1_->get(key, res);
        expires_->update(key);
        lkS.unlock();
        return;
    }
    hash2_->get(key, res);
    expires_->update(key);
    lkS.unlock();
    std::unique_lock<std::shared_mutex> lk(smutex_);
    progressiveRehash(hash2_);
    lk.unlock();
}

int MiniKVDB::del(std::string key) {
    std::unique_lock<std::shared_mutex> lk(smutex_);
    int flag;
    if (rehashFlag_) {
        int f1 = hash1_->del(key);
        int f2 = hash2_->del(key);
        expires_->del(key);
        flag = (f1 == MiniKV_DEL_SUCCESS || f2 == MiniKV_DEL_SUCCESS);
    } else {
        int f1 = hash1_->del(key);
        expires_->del(key);
        flag = (f1 == MiniKV_DEL_SUCCESS);
    }
    return flag ? MiniKV_DEL_SUCCESS : MiniKV_DEL_FAIL;
}

void MiniKVDB::getKeyName(std::string keyRex, std::vector<std::string>& res) {
    std::shared_lock<std::shared_mutex> lk(smutex_);
    if (!rehashFlag_) {
        hash1_->findKeyName(keyRex, res);
        return;
    }
    hash2_->findKeyName(keyRex, res);
}

int MiniKVDB::setExpire(std::string key, uint64_t expires) {
    bool exist = hash1_->exist(key);
    if (!exist) return MiniKV_SET_EXPIRE_FAIL;
    std::unique_lock<std::shared_mutex> lk(smutex_);
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

uint32_t MiniKVDB::getVmrss() {
    int pid = getpid();
    std::string file_name = "/proc/" + to_string(pid) + "/status";
    ifstream file(file_name);
    std::string line;
    uint32_t vmrss = 0;
    while (getline(file, line)) {
        if (line.find("VmRSS") != string::npos)
        {
            vmrss = stoi(line.substr(line.find(" ")));
            break;
        } 
    }
    return vmrss;
}

void MiniKVDB::memoryDetector() {
    uint32_t vmrss = getVmrss();
    if (vmrss > MAX_MEMORY_KB) {
        memoryObsolescence();
    }
}

void MiniKVDB::memoryObsolescence() {
    std::unique_lock<std::shared_mutex> lk(smutex_);
    int keyNum = expires_->keyNum();
    for (int i = 0; i < keyNum; ++i) {
        std::string waitingToDelKey = expires_->lru_.lastKey();
        hash1_->del(waitingToDelKey);
        hash2_->del(waitingToDelKey);
        expires_->del(waitingToDelKey);
        if (i % 10 && getVmrss() <= MAX_MEMORY_KB) {
            break;
        }
    }
}