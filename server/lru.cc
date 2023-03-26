#include "lru.h"

DList::DList() {
    head_ = new Node;
    tail_ = new Node;
    head_->next = tail_;
    head_->prev = tail_;
    tail_->next = head_;
    tail_->prev = head_;
    count_ = 0;
}

DList::~DList() {
    Node* t = head_;
    while (t) {
        Node* nxt = t->next;
        delete t;
        t = nxt;
    }
}

void DList::insertFront(const std::string& key) {
    Node* n = new Node{key};
    n->next = head_->next;
    n->prev = head_;
    head_->next->prev = n;
    head_->next = n;
    count_++;
}

void DList::insertFront(Node* n) {
    n->next = head_->next;
    n->prev = head_;
    head_->next->prev = n;
    head_->next = n;
    count_++;
}

bool DList::empty() {
    return size() == 0;
}

int DList::size() {
    return count_;
}

Node* DList::tail() {
    if (tail_->prev == head_) return nullptr;
    return tail_->prev;
}

void DList::popBack() {
    Node* t = tail_->prev;
    t->prev->next = tail_;
    tail_->prev = t->prev;
    delete t;
    count_--;
}

void DList::remove(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    count_--;
    // mark: do not delete node, because of it will insert to the front of the dlist then
}

LRUCache::LRUCache() {
    dlist_ = new DList;
}

LRUCache::~LRUCache() {
    delete dlist_;
}

std::string LRUCache::lastKey() {
    if (dlist_->empty()) {
        throw MiniKVLogicErr();
    }
    return dlist_->tail()->key;
}

void LRUCache::update(const std::string& key) {
    if (keyMap_.count(key) == 0) {
        throw MiniKVLogicErr();
    }
    dlist_->remove(keyMap_[key]);
    dlist_->insertFront(keyMap_[key]);
}

void LRUCache::remove(const std::string& key) {
    if (keyMap_.count(key) == 0) {
        throw MiniKVLogicErr();
    }
    dlist_->remove(keyMap_[key]);
    auto p = keyMap_[key];
    keyMap_.erase(key);
    delete p;
}

void LRUCache::insert(const std::string& key) {
    if (keyMap_.count(key) != 0) {
        throw MiniKVLogicErr();
    }
    Node* n = new Node{key};
    keyMap_[key] = n;
    dlist_->insertFront(n);
}