#ifndef HASH_H
#define HASH_H

#include <list>
#include <memory>
#include <algorithm>
#include <vector>
#include <regex>
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
    std::size_t keyNum_;
    int size_;
    int rehashIndex_;
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
        keyNum_ = rehashIndex_ = 0;
        size_ = size;
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
        keyNum_++;
    }

    void insertEntry(std::shared_ptr<Entry> entry, std::string key) {
        int slot = hash(key);
        hash_[slot].push_front(entry);
        keyNum_++;
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
    bool exist(std::string key) {
        int slot = hash(key);
        for (auto i = hash_[slot].begin(); i != hash_[slot].end(); ++i) {
            if (i->get()->key == key) {
                return true;
            }
        }
        return false;
    }
    int del(std::string key) {
        int slot = hash(key);
        for (auto it = hash_[slot].begin(); it != hash_[slot].end(); ++it) {
            if (it->get()->key == key) {
                hash_[slot].remove(*it);
                return MiniKV_DEL_SUCCESS;
            }
        }
        return MiniKV_DEL_FAIL;
    }

    bool needRehash() {
        if ((double)keyNum_ / size_ > 1.5) {
            return true;
        }
        return false;
    }

    void resize(int size) {
        hash_.resize(size);
        size_ = size;
    }

    size_t size() {
        return size_;
    }

    void clear() {
        keyNum_ = rehashIndex_ = 0;
        hash_.clear();
    }

    bool progressiveRehash(std::shared_ptr<HashTable> h2, int emptyVisits) {
        auto& d2 = h2.get()->hash_;
        auto& d1 = hash_;
        for (int i = rehashIndex_; i < d1.size() && emptyVisits > 0; ++i) {
            if (d1[i].empty()) {
                emptyVisits--;
                rehashIndex_ = i + 1;
                continue;
            }
            for (auto it = d1[i].begin(); it != d1[i].end(); ++it) {
                std::string key = (*it).get()->key;
                h2->insertEntry(*it, key);
            }
            d1[i].clear();
            rehashIndex_ = i + 1;
        }
        if (rehashIndex_ == d1.size()) {
            return true;
        }
        return false;
    }

    void findKeyName(std::string keyRex, std::vector<std::string>& res) {
        std::regex pattern(keyRex);
        for (int i = 0; i < hash_.size(); ++i) {
            for (auto it = hash_[i].begin(); it != hash_[i].end(); ++it) {
                if (std::regex_search(it->get()->key, pattern)) {
                    res.push_back(it->get()->key);
                }
            }
        }
    }
};

#endif