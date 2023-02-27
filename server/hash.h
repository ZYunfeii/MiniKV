#ifndef HASH_H
#define HASH_H

#include <list>
#include <memory>
#include <algorithm>
#include <vector>
#include "../type/encoding.h"

typedef struct Entry {
    uint32_t encoding;
    std::string key;
    void* data;
    ~Entry() {
        switch (encoding) {
            case MiniKV_STRING: {
                std::string* strp = (std::string*)data;
                delete strp;
                break;
            }
            case MiniKV_LIST: {
                std::list<std::string>* listp = (std::list<std::string>*)data;
                delete listp;
                break;
            }
        }
    }
} Entry;

class HashTable {
private:
    std::vector<std::list<std::shared_ptr<Entry>>> hash_;
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
                entry->data = new std::string(val);
                break;
            }
            case MiniKV_LIST : {
                auto node = new std::list<std::string>;
                node->push_back(val);
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
                insertWithEncoding(i->get(), key, val, encoding);
                return;
            }
        }
        Entry* entry = new Entry;
        insertWithEncoding(entry, key, val, encoding);
        hash_[slot].push_front(std::shared_ptr<Entry>(entry)); 
    }

    std::vector<std::string> get(std::string key) {
        int slot = hash(key);
        std::vector<std::string> res;
        for (auto i = hash_[slot].begin(); i != hash_[slot].end(); ++i) {
            if (i->get()->key == key) {
                if (i->get()->encoding == MiniKV_STRING) {
                    res.push_back(*(std::string*)(i->get()->data));
                    return res;
                }
                if (i->get()->encoding == MiniKV_LIST) {
                    std::list<std::string>* p = (std::list<std::string>*)(i->get()->data);
                    if (p->empty()) {
                        return {};
                    } else {
                        for (auto it = p->begin(); it != p->end(); ++it) {
                            res.push_back(*it);
                        }
                        return res;
                    }
                }
            }
        }
        
        return {};
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