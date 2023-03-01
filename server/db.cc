#include "db.h"
std::shared_mutex smutex_;
ThreadPool threadpool(THREAD_POOL_NUM);
// Although sizeof can be obtained at compile time
static constexpr uint32_t fixedEntryHeadLen = 3 * sizeof(uint32_t);
static constexpr uint32_t sizeofUint32 = sizeof(uint32_t);
static constexpr uint32_t sizeofChar = sizeof(char);

MiniKVDB::MiniKVDB() {
    hashSize_ = HASH_SIZE_INIT;
    hash1_ = std::unique_ptr<HashTable>(new HashTable(hashSize_));
    io_ = std::unique_ptr<KVio>(new KVio(RDB_FILE_NAME));
    rdbFileReadInitDB();
    rdbe_ = new rdbEntry();
    timer_ = std::shared_ptr<KVTimer>(new KVTimer());
    timer_->start(RDB_INTERVAL, std::bind(&MiniKVDB::rdbSave, this));
}

MiniKVDB::~MiniKVDB() {
    timer_->stop();
    io_->flushCache();
    delete rdbe_;
}

void MiniKVDB::insert(std::string key, std::string val, uint32_t encoding) {
    hash1_->insert(key, val, encoding);
}

void MiniKVDB::get(std::string key, std::vector<std::string>& res) {
    hash1_->get(key, res);
}

int MiniKVDB::del(std::string key) {
    return hash1_->del(key);
}

void MiniKVDB::rdbSave() {
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
    if (io_->emptyFile()) {
        kvlogi("No rdb file to load.");
        return;
    }
    while (!io_->reachEOF()) {
        rdbLoadEntry();
    }
    kvlogi("Rdb file loaded.");
}