#ifndef KV_SERVER_H
#define KV_SERVER_H

#include <map>
#include <string>
#include <functional>
#include <grpcpp/grpcpp.h>
#include <mutex>
#include <shared_mutex>
#include "../proto/kvserver.grpc.pb.h"
#include "hash.h"

class KVService final : public kv::KVServer::Service {
private:
    std::shared_mutex smutex_;
    grpc::Status SetKV(grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res) override {
        std::unique_lock<std::shared_mutex> lk(smutex_);
        grpc::Status status = setCallback_(context, req, res);
        return status;
    }
    grpc::Status GetKV(grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res) override {
        std::shared_lock<std::shared_mutex> lk(smutex_);
        grpc::Status status = getCallback_(context, req, res);
        return status;
    }
public:
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res)> setCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res)> getCallback;
    setCallback setCallback_;
    getCallback getCallback_;
    void setSetKVCallback(setCallback cb) {
        setCallback_ = cb;
    }
    void setGetKCallback(getCallback cb) {
        getCallback_ = cb;
    }
};



#define HASH_SIZE_INIT 512

class KVServer {
private:
    std::unique_ptr<HashTable> hash1_;
    std::unique_ptr<HashTable> hash2_;
    std::string ip_;
    uint32_t port_;
    KVService* kvService_;
    uint64_t hashSize_;
    void serviceCallbackSet();
public:
    KVServer(std::string ip, uint32_t port);
    void serve();
};

#endif