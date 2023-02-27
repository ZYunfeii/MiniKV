#include "db.h"
std::shared_mutex smutex_;
MiniKVDB::MiniKVDB() {
    hashSize_ = HASH_SIZE_INIT;
    hash1_ = std::unique_ptr<HashTable>(new HashTable(hashSize_));
    io_ = std::unique_ptr<KVio>(new KVio(RDB_FILE_NAME));
}

void MiniKVDB::insert(std::string key, std::string val, uint32_t encoding) {
    hash1_->insert(key, val, encoding);
}

void MiniKVDB::get(std::string key, std::vector<std::string>& res) {
    hash1_->get(key, res);
}

int MiniKVDB::rdbSave() {
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
    return 1;
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

    delete rdbe;
}

void MiniKVDB::makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {
    uint32_t keyLen = (uint32_t)key.size();
    kvString* kvStr = (kvString*)data;
    uint32_t totalLen = sizeof(uint32_t) * 3 + keyLen + sizeof(uint32_t) + kvStr->len;
    rdbe->totalLen = totalLen;
    rdbe->encoding = encoding;
    rdbe->keyLen = keyLen;
    rdbe->key = key;
    
}

void MiniKVDB::makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data) {

}