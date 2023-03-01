#ifndef DB_H
#define DB_H

#include "hash.h"
#include "io.h"
#include "../threadpool/threadpool.h"
#include "../timer/timer.h"
#include <mutex>
#include <shared_mutex>
#define HASH_SIZE_INIT 512
#define THREAD_POOL_NUM 4
#define RDB_INTERVAL 5000 // 5000ms rdb save triggered
#define RDB_FILE_NAME "minikv.rdb"


#define CHECK_REST_AND_LOG(x)          \
    do {                               \
        if (x < 0) {                \
            kvloge("rdb read error!"); \
        }                              \
    } while (0)

extern std::shared_mutex smutex_;
extern ThreadPool threadpool;
typedef struct rdbEntry {
    uint32_t totalLen;
    uint32_t encoding;
    uint32_t keyLen;
    std::shared_ptr<char[]> key;
    std::shared_ptr<char[]> data;
} rdbEntry;
 

typedef class MiniKVDB {
private:
    std::unique_ptr<HashTable> hash1_;
    std::unique_ptr<HashTable> hash2_; 
    std::unique_ptr<HashTable> expires_;
    uint64_t hashSize_; 
    std::unique_ptr<KVio> io_;
    std::shared_ptr<KVTimer> timer_;
    rdbEntry* rdbe_;

    void saveKVWithEncoding(std::string key, uint32_t encoding, void* data);
    void makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
    void makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
    void rdbEntryWrite(rdbEntry* rdbe);
    void rdbFileReadInitDB();
    void rdbLoadEntry();
    void rdbSave();
    // void fixedTimeDeleteExpiredKey();
public:
    void insert(std::string key, std::string val, uint32_t encoding);
    void get(std::string key, std::vector<std::string>& res);
    int del(std::string key);
    int setExpire(std::string key, uint64_t expires);
    MiniKVDB();
    ~MiniKVDB();

} MiniKVDB;

#endif