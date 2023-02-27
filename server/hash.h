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
    std::string data;
} kvString;
typedef struct Entry {
    uint32_t encoding;
    std::string key;
    void* data;
    ~Entry() {
        switch (encoding) {
            case MiniKV_STRING: {
                kvString* strp = (kvString*)data;
                delete strp;
                break;
            }
            case MiniKV_LIST: {
                std::list<kvString>* listp = (std::list<kvString>*)data;
                delete listp;
                break;
            }
        }
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
                entry->data = new kvString{(uint32_t)val.size(), val};
                break;
            }
            case MiniKV_LIST : {
                auto node = new std::list<kvString>;
                node->push_back(kvString{(uint32_t)val.size(), val});
                entry->encoding = encoding;
                entry->key = key;
                entry->data = node;
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
                    std::list<kvString>* p = (std::list<kvString>*)(i->get()->data);
                    p->push_front(kvString{(uint32_t)val.size(), val});
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
                    res.push_back(((kvString*)(i->get()->data))->data);
                }
                if (i->get()->encoding == MiniKV_LIST) {
                    std::list<kvString>* p = (std::list<kvString>*)(i->get()->data);
                    if (p->empty()) {
                    } else {
                        for (auto it = p->begin(); it != p->end(); ++it) {
                            res.push_back(it->data);
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