#ifndef DB_H
#define DB_H

#include <sys/resource.h>
#include <fstream>
#include <unistd.h>
#include "hash.h"
#include "io.h"
#include "../threadpool/threadpool.h"
#include "../timer/timer.h"
#include <mutex>
#include <shared_mutex>
#define HASH_SIZE_INIT 512
#define THREAD_POOL_NUM 2
#define RDB_INTERVAL 5000 // 5000ms rdb save triggered
#define REHASH_DETECT_INTERVAL 5000
#define REHASH_MAX_EMPTY_VISITS 50
#define FIXED_TIME_DELETE_EXPIRED_KEY 200
#define ACTIVE_EXPIRE_CYCLE_LOOKUPS_PER_LOOP 20
#define MEMORY_DETECT_INTERVAL 1000

#define MAX_MEMORY_KB 100000 // kb
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
    std::shared_ptr<HashTable> hash1_;
    std::shared_ptr<HashTable> hash2_; 
    std::unique_ptr<HashTable> expires_;
    uint64_t hashSize_; 
    std::unique_ptr<KVio> io_;
    std::shared_ptr<KVTimer> timerRdb_;
    std::shared_ptr<KVTimer> timerRehash_;
    std::shared_ptr<KVTimer> timerFixedTimeDelKey_;
    std::shared_ptr<KVTimer> timerMemoryDetect_;
    rdbEntry* rdbe_;
    bool rehashFlag_;

    void saveKVWithEncoding(std::string key, uint32_t encoding, void* data);
    void makeStringObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
    void makeListObject2RDBEntry(rdbEntry* rdbe, std::string key, uint32_t encoding, void* data);
    void rdbEntryWrite(rdbEntry* rdbe);
    void rdbFileReadInitDB();
    void rdbLoadEntry();
    void rdbSave();
    void rehash();
    void progressiveRehash(std::shared_ptr<HashTable> hash2);
    bool expired(std::string key);
    void fixedTimeDeleteExpiredKey();
    void memoryDetector();
    void memoryObsolescence();
    uint32_t getVmrss();
public:
    void insert(std::string key, std::string val, uint32_t encoding);
    void get(std::string key, std::vector<std::string>& res);
    int del(std::string key);
    int setExpire(std::string key, uint64_t expires);
    void getKeyName(std::string keyRex, std::vector<std::string>& res);
    MiniKVDB();
    ~MiniKVDB();

} MiniKVDB;

#endif