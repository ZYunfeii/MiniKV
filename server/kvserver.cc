
#include "kvserver.h"

KVServer::KVServer(std::string ip, uint32_t port) : ip_(ip), port_(port){
    kvService_ = std::unique_ptr<KVService>(new KVService());
    db_ = std::unique_ptr<MiniKVDB>(new MiniKVDB);
    serviceCallbackSet();
}

void KVServer::serviceCallbackSet() {
    kvService_->setSetKVCallback([this](grpc::ServerContext* context, const kv::ReqKV* req, kv::SetKVResponse* res)->grpc::Status{
        uint32_t encoding = req->encoding();
        std::string key = req->key();
        std::string val = req->val();
        std::unique_lock<std::shared_mutex> lk(smutex_);
        db_->insert(key, val, encoding);
        lk.unlock();
        res->set_flag(true);
        return grpc::Status::OK;
    });

    kvService_->setGetKCallback([this](grpc::ServerContext* context, const kv::ReqK* req, kv::GetKResponse* res)->grpc::Status{
        std::string key = req->key();
        std::vector<std::string> v;
        std::shared_lock<std::shared_mutex> lk(smutex_);
        db_->get(key, v);
        lk.unlock();
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

    kvService_->setDelCallback([this](grpc::ServerContext* context, const kv::ReqK* req, kv::DelKVResponse* res)->grpc::Status{
        std::string key = req->key();
        std::shared_lock<std::shared_mutex> lk(smutex_);
        int flag = db_->del(key);
        lk.unlock();
        if (flag == MiniKV_DEL_FAIL) {
            res->set_flag(false);
        } else {
            res->set_flag(true);
        }
        return grpc::Status::OK;
    });
}

void KVServer::serve() {
    grpc::ServerBuilder builder;
    std::string serverAddr = ip_ + ":" + std::to_string(port_);
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(kvService_.get());
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    fprintf(stdout, "Server listening on:%s", serverAddr.data());
    fflush(stdout);
    server->Wait();
}