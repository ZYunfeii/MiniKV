
#include "kvserver.h"

KVServer::KVServer(std::string ip, uint32_t port) : ip_(ip), port_(port){
    kvService_ = new KVService();
    hashSize_ = HASH_SIZE_INIT;
    hash1_ = std::unique_ptr<HashTable>(new HashTable(hashSize_));
    serviceCallbackSet();
}

void KVServer::serviceCallbackSet() {
    kvService_->setSetKVCallback([this](grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res)->grpc::Status{
        uint32_t encoding = req->encoding();
        std::string key = req->key();
        std::string val = req->val();
        hash1_->insert(key, val, encoding);
        res->set_flag(true);
        return grpc::Status::OK;
    });

    kvService_->setGetKCallback([this](grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res)->grpc::Status{
        std::string key = req->key();
        std::vector<std::string> v = hash1_->get(key);
        if (v.empty()) {
            res->set_flag(false);
            return grpc::Status::OK;
        }
        res->set_flag(true);
        for (auto& str : v) {
            res->add_val(str);
        }
        return grpc::Status::OK;
    });
}

void KVServer::serve() {
    grpc::ServerBuilder builder;
    std::string serverAddr = ip_ + ":" + std::to_string(port_);
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(kvService_);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    fprintf(stdout, "Server listening on:%s", serverAddr.data());
    fflush(stdout);
    server->Wait();
}