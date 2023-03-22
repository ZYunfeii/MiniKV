#ifndef KV_SERVER_H
#define KV_SERVER_H

#include <map>
#include <string>
#include <functional>
#include <grpcpp/grpcpp.h>
#include "../proto/kvserver.grpc.pb.h"
#include "db.h"
extern std::shared_mutex smutex_;
class KVService final : public kv::KVServer::Service {
private:
    grpc::Status SetKV(grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res) override {      
        grpc::Status status = setCallback_(context, req, res);
        return status;
    }
    grpc::Status GetKV(grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res) override {
        grpc::Status status = getCallback_(context, req, res);
        return status;
    }
    grpc::Status DelKV(grpc::ServerContext* context, const kv::ReqK* req, kv::DelKVResponse* res) override {
        grpc::Status status = delCallback_(context, req, res);
        return status;
    }
    grpc::Status SetExpire(grpc::ServerContext* context, const kv::ReqExpire* req, kv::SetExpireResponse* res) override {
        grpc::Status status = expireCallback_(context, req, res);
        return status;
    }
    grpc::Status GetKeyName(grpc::ServerContext* context, const kv::ReqKeyName* req, kv::GetKeyNameResponse* res) override {
        grpc::Status status = getKeyNameCallback_(context, req, res);
        return status;
    }
    grpc::Status SetKVStream(grpc::ServerContext* context, grpc::ServerReaderWriter<kv::SetKVResponse, kv::ReqKV>* stream) override {
        grpc::Status status = setStreamCallback_(context, stream);
        return status;
    }
public:
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res)> setCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res)> getCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqK* req, kv::DelKVResponse* res)> delCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqExpire* req, kv::SetExpireResponse* res)> expireCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, const kv::ReqKeyName* req, kv::GetKeyNameResponse* res)> getKeyNameCallback;
    typedef std::function<grpc::Status(grpc::ServerContext* context, grpc::ServerReaderWriter<kv::SetKVResponse, kv::ReqKV>* stream)> setStreamCallback;
    setCallback setCallback_;
    getCallback getCallback_;
    delCallback delCallback_;
    expireCallback expireCallback_;
    getKeyNameCallback getKeyNameCallback_;
    setStreamCallback setStreamCallback_;
    void setSetKVCallback(setCallback cb) {
        setCallback_ = cb;
    }
    void setGetKCallback(getCallback cb) {
        getCallback_ = cb;
    }
    void setDelCallback(delCallback cb) {
        delCallback_ = cb;
    }
    void setExpireCallback(expireCallback cb) {
        expireCallback_ = cb;
    }
    void setGetKeyNameCallback(getKeyNameCallback cb) {
        getKeyNameCallback_ = cb;
    }
    void setSetStreamCallback(setStreamCallback cb) {
        setStreamCallback_ = cb;
    }
};


class KVServer {
private:
    std::unique_ptr<MiniKVDB> db_;
    std::string ip_;
    uint32_t port_;
    std::unique_ptr<KVService> kvService_;
    std::unique_ptr<grpc::Server> server_;
    void serviceCallbackSet();
public:
    KVServer(std::string ip, uint32_t port);
    void serve();
    void gracefullyStop();
};

#endif