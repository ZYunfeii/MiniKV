#ifndef HASH_H
#define HASH_H

#include <list>
#include <memory>
#include <algorithm>
#include <vector>
#include "../type/encoding.h"
#include "../log/log.h"

typedef struct kvString {
    uint32_t len;
    std::shared_ptr<char[]> data;
} kvString;
typedef struct Entry {
    uint32_t encoding;
    std::string key;
    std::shared_ptr<char[]> data;
    ~Entry() {
    }
} Entry;

class HashTable {
public:
    std::vector<std::list<std::shared_ptr<Entry>>> hash_;
private:
    int hash(std::string key) {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;
        char* str = const_cast<char*>(key.data());
        while (*str) {
            hash = hash * seed + (*str++);
        }
        return (hash & 0x7FFFFFFF) % hash_.size();
    }
    inline void insertWithEncoding(Entry* entry, std::string key, std::string val, uint32_t encoding) {
        switch (encoding) {
            case MiniKV_STRING : {
                entry->encoding = encoding;
                entry->key = key;
                char* valBuf = new char[val.size()];
                memcpy(valBuf, val.data(), val.size());
                entry->data = std::shared_ptr<char[]>(
                                (char*)new kvString{(uint32_t)val.size(), 
                                std::shared_ptr<char[]>(valBuf)}
                                );
                break;
            }
            case MiniKV_LIST : {
                auto node = new std::list<kvString>;
                char* valBuf = new char[val.size()];
                memcpy(valBuf, val.data(), val.size());
                node->push_back(kvString{(uint32_t)val.size(), 
                                          std::shared_ptr<char[]>(valBuf)});
                entry->encoding = encoding;
                entry->key = key;
                entry->data = std::shared_ptr<char[]>((char*)node);
                break;
            }
        }
    }
public:
    HashTable() = delete;
    HashTable(int size) {
        hash_.resize(size);
    }
    void insert(std::string key, std::string val, uint32_t encoding) {
        int slot = hash(key);
        for (auto i = hash_[slot].begin(); i != hash_[slot].end(); ++i) {
            if (i->get()->key == key) {
                if (i->get()->encoding == MiniKV_LIST) {
                    std::list<kvString>* p = (std::list<kvString>*)(i->get()->data.get());
                    char* valBuf = new char[val.size()];
                    memcpy(valBuf, val.data(), val.size());
                    p->push_front(kvString{(uint32_t)val.size(), std::shared_ptr<char[]>(valBuf)});
                } else if (i->get()->encoding == MiniKV_STRING) {
                    insertWithEncoding(i->get(), key, val, encoding);
                }
                return;
            }
        }
        Entry* entry = new Entry;
        insertWithEncoding(entry, key, val, encoding);
        hash_[slot].push_front(std::shared_ptr<Entry>(entry)); 
    }

    void get(std::string key, std::vector<std::string>& res) {
        int slot = hash(key);
        for (auto i = hash_[slot].begin(); i != hash_[slot].end(); ++i) {
            if (i->get()->key == key) {
                if (i->get()->encoding == MiniKV_STRING) {
                    kvString* kvStr = (kvString*)(i->get()->data.get());
                    std::string s(kvStr->data.get(), kvStr->data.get() + kvStr->len);
                    res.push_back(s);
                }
                if (i->get()->encoding == MiniKV_LIST) {
                    std::list<kvString>* p = (std::list<kvString>*)(i->get()->data.get());
                    if (p->empty()) {
                    } else {
                        for (auto it = p->begin(); it != p->end(); ++it) {
                            std::string s(it->data.get(), it->data.get() + it->len);
                            res.push_back(s);
                        }
                    }
                }
            }
        }   
    }
    void del(std::string key) {
        int slot = hash(key);
        std::for_each(hash_[slot].begin(), hash_[slot].end(), [&key, &slot, this](std::shared_ptr<Entry> p) {
            if (p.get()->key == key) {
                hash_[slot].remove(p);
            }
        });
    }
};

#endif