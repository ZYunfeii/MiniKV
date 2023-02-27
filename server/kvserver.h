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


class KVServer {
private:
    std::unique_ptr<MiniKVDB> db_;
    std::string ip_;
    uint32_t port_;
    std::unique_ptr<KVService> kvService_;
    void serviceCallbackSet();
public:
    KVServer(std::string ip, uint32_t port);
    void serve();
};

#endif