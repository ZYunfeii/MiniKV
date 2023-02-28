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
    timer_ = std::shared_ptr<KVTimer>(new KVTimer());
    timer_->start(RDB_INTERVAL, std::bind(&MiniKVDB::rdbSave, this));
}

void MiniKVDB::insert(std::string key, std::string val, uint32_t encoding) {
    hash1_->insert(key, val, encoding);
}

void MiniKVDB::get(std::string key, std::vector<std::string>& res) {
    hash1_->get(key, res);
}

void MiniKVDB::rdbSave() {
    std::unique_lock<std::shared_mutex> lk(smutex_);
    kvlogi("rdb save triggered!");
    for (int i = 0; i < hash1_->hash_.size(); ++i) {
        auto d = hash1_->hash_[i];
        if (d.empty()) {
            continue;
        }
        for (auto it = d.begin(); it != d.end(); ++it) {
            std::string key = it->get()->key;
            uint32_t encoding = it->get()->encoding;
            void* data = it->get()->data;
            saveKVWithEncoding(key, encoding, data);
        }
    }
    io_->flushCache();
}

void MiniKVDB::saveKVWithEncoding(std::string key, uint32_t encoding, void* data) {
    rdbEntry* rdbe = new rdbEntry;
    switch (encoding) {
        case MiniKV_STRING : {
            makeStringObject2RDBEntry(rdbe, key, encoding, data);
            break;
        }
        case MiniKV_LIST : {
            makeListObject2RDBEntry(rdbe, key, encoding, data);
            break;
        }
    }
    rdbEntryWrite(rdbe);
    delete rdbe;
}

void MiniKVDB::makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {
    uint32_t keyLen = (uint32_t)key.size();
    kvString* kvStr = (kvString*)data;
    uint32_t totalLen = fixedEntryHeadLen + keyLen + sizeofUint32 + kvStr->len;
    rdbe->totalLen = totalLen;
    rdbe->encoding = encoding;
    rdbe->keyLen = keyLen;
    rdbe->key = key.data();
    char tmp[sizeofUint32 + strlen(kvStr->data)];
    memcpy(tmp, kvStr, sizeofUint32);
    memcpy(tmp + sizeofUint32, kvStr->data, kvStr->len);
    rdbe->data = tmp;
}

void MiniKVDB::makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {
    uint32_t keyLen = (uint32_t)key.size();
    auto lst = (std::list<kvString>*)data;
    uint32_t totalLen = fixedEntryHeadLen + keyLen;
    for (auto it = lst->begin(); it != lst->end(); ++it) {
        char tmp[sizeofUint32 + strlen(it->data)];
        memcpy(tmp, (char*)(&(*it)), sizeofUint32);
        memcpy(tmp + sizeofUint32, it->data, it->len);
        rdbe->data = tmp;
        totalLen += sizeofUint32 + it->len;
    }
    rdbe->totalLen = totalLen;
    rdbe->encoding = encoding;
    rdbe->keyLen = keyLen;
    rdbe->key = key.data();
}

void MiniKVDB::rdbEntryWrite(rdbEntry* rdbe) {
    int t = io_->kvioFileWrite(rdbe, fixedEntryHeadLen); // write totalLen, encoding and keyLen
    if (t < 0) {
        kvloge("rdb write error: key:%s", rdbe->key);
        return;
    }
    size_t keyLen = rdbe->keyLen;
    size_t dataLen = rdbe->totalLen - fixedEntryHeadLen - keyLen;
    t = io_->kvioFileWrite(rdbe->key, keyLen);
    if (t < 0) {
        kvloge("rdb write error: key:%s", rdbe->key);
        return;
    }
    t = io_->kvioFileWrite(rdbe->data, dataLen);
    if (t < 0) {
        kvloge("rdb write error: key:%s", rdbe->key);
        return;
    }
}