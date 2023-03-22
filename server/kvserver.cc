
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
        std::unique_lock<std::shared_mutex> lk(smutex_);
        int flag = db_->del(key);
        lk.unlock();
        if (flag == MiniKV_DEL_FAIL) {
            res->set_flag(false);
        } else {
            res->set_flag(true);
        }
        return grpc::Status::OK;
    });

    kvService_->setExpireCallback([this](grpc::ServerContext* context, const kv::ReqExpire* req, kv::SetExpireResponse* res)->grpc::Status{
        std::string key = req->key();
        uint64_t expires = req->expires();
        // TODO: use another lock to lock the expire hash table
        std::unique_lock<std::shared_mutex> lk(smutex_);
        int flag = db_->setExpire(key, expires);
        lk.unlock();
        res->set_flag(flag);
        return grpc::Status::OK;
    });

    kvService_->setGetKeyNameCallback([this](grpc::ServerContext* context, const kv::ReqKeyName* req, kv::GetKeyNameResponse* res)->grpc::Status{
        std::string keyRex = req->keyrex();
        std::shared_lock<std::shared_mutex> lk(smutex_);
        std::vector<std::string> ans;
        db_->getKeyName(keyRex, ans);
        lk.unlock();
        for (auto& key : ans) {
            res->add_val(key);
        }
        return grpc::Status::OK;
    });

    kvService_->setSetStreamCallback([this](grpc::ServerContext* context, grpc::ServerReaderWriter<kv::SetKVResponse, kv::ReqKV>* stream)->grpc::Status{
        kv::ReqKV req;
        kv::SetKVResponse res;
        std::unique_lock<std::shared_mutex> lk(smutex_);
        while (stream->Read(&req)) {
            std::string key = req.key();
            std::string val = req.val();
            uint32_t encoding = req.encoding();
            db_->insert(key, val, encoding);
            res.set_flag(true);
            stream->Write(res);
        }
        lk.unlock();
        return grpc::Status::OK;
    });
}

void KVServer::serve() {
    grpc::ServerBuilder builder;
    std::string serverAddr = ip_ + ":" + std::to_string(port_);
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(kvService_.get());
    server_ = std::unique_ptr<grpc::Server>(builder.BuildAndStart());
    fprintf(stdout, "Server listening on:%s", serverAddr.data());
    fflush(stdout);
    server_->Wait();
}

void KVServer::gracefullyStop() {
    server_->Shutdown();
}