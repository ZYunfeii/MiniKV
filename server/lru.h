#ifndef LRU_H
#define LRU_H
#include <string>
#include <unordered_map>
#include "../exception/minikv_exception.h"
typedef struct Node {
    std::string key;
    Node* next;
    Node* prev;
} LRUNode;

typedef class DList {
public:
    DList();
    ~DList();
    bool empty();
    void insertFront(const std::string& key);
    void insertFront(Node* node);
    Node* tail();
    void popBack();
    void remove(Node* node);
    int size();
private:
    int count_;
    Node* head_;
    Node* tail_;

} DList;
// not to set max size of lru, because of already have memory policy
// the tail of lru cache is the node waiting to delete
typedef class LRUCache {
public:
    LRUCache();
    ~LRUCache();
    std::string lastKey();
    void update(const std::string& key);
    void remove(const std::string& key);
    void insert(const std::string& key);
private:
    std::unordered_map<std::string, Node*> keyMap_;
    DList* dlist_;
    
} LRUCache;

#endif