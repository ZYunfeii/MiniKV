#ifndef DB_H
#define DB_H

#include "hash.h"
#include "io.h"
#include <mutex>
#include <shared_mutex>
#define HASH_SIZE_INIT 512
#define RDB_FILE_NAME "minikv.rdb"
extern std::shared_mutex smutex_;

typedef struct rdbEntry {
    uint32_t totalLen;
    uint32_t encoding;
    uint32_t keyLen;
    std::string key;
    std::string data;
} rdbEntry;

typedef class MiniKVDB {
private:
    std::unique_ptr<HashTable> hash1_;
    std::unique_ptr<HashTable> hash2_; 
    uint64_t hashSize_; 
    std::unique_ptr<KVio> io_;  
    void saveKVWithEncoding(std::string key, uint32_t encoding, void* data);
    void makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
    void makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
public:
    void insert(std::string key, std::string val, uint32_t encoding);
    void get(std::string key, std::vector<std::string>& res);
    MiniKVDB();
    int rdbSave();

} MiniKVDB;

#endif