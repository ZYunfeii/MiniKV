#include "kvclient.h"

KVClient::KVClient(std::string ip, uint32_t port) : ip_(ip), port_(port) {
    std::string serverAddr = ip_ + ":" + std::to_string(port_);
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(serverAddr, grpc::InsecureChannelCredentials());
    stub_ = kv::KVServer::NewStub(channel);
}

int KVClient::setKV(std::string key, std::string val, uint32_t encoding) {
    if (key.empty()) return MiniKV_KEY_EMPTY;
    kv::ReqKV req;
    kv::SetKVResponse res;
    req.set_key(key);
    req.set_val(val);
    req.set_encoding(encoding);
    grpc::ClientContext context;
    grpc::Status status = stub_->SetKV(&context, req, &res);
    if (!status.ok() || res.flag() == false) {
        // TODO: log
        return MiniKV_SET_FAIL;
    }
    return MiniKV_SET_SUCCESS;
}

int KVClient::delK(std::string key) {
    if (key.empty()) return MiniKV_KEY_EMPTY;
    kv::ReqK req;
    kv::DelKVResponse res;
    req.set_key(key);
    grpc::ClientContext context;
    grpc::Status status = stub_->DelKV(&context, req, &res);
    if (!status.ok() || res.flag() == false) {
        // TODO: log
        return MiniKV_DEL_FAIL;
    }
    return MiniKV_DEL_SUCCESS;
}

int KVClient::getK(std::string key, std::vector<std::string>& ans) {
    if (key.empty()) return MiniKV_KEY_EMPTY;
    kv::ReqK req;
    kv::GetKResponse res;
    req.set_key(key);
    grpc::ClientContext context;
    grpc::Status status = stub_->GetKV(&context, req, &res);
    if (!status.ok() || res.flag() == false) {
        // TODO: log
        ans = {};
        return MiniKV_GET_FAIL;
    }
    auto p = res.val();
    for (auto it = p.begin(); it != p.end(); ++it) {
        ans.push_back(it->data());
    }
    return MiniKV_GET_SUCCESS;
}

int KVClient::setExpires(std::string key, uint64_t millisecond) {
    if (key.empty()) return MiniKV_KEY_EMPTY;
    auto now = std::chrono::system_clock::now(); 
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    uint64_t expires = timestamp + millisecond;
    kv::ReqExpire req;
    kv::SetExpireResponse res;
    req.set_key(key);
    req.set_expires(expires);
    grpc::ClientContext context;
    grpc::Status status = stub_->SetExpire(&context, req, &res);
    if (!status.ok() || res.flag() == false) {
        // TODO: log
        return MiniKV_SET_EXPIRE_FAIL;
    }
    return MiniKV_SET_EXPIRE_SUCCESS;
}

int KVClient::getKeyName(std::string key, std::vector<std::string>& ans) {
    if (key.empty()) return MiniKV_KEY_EMPTY;
    kv::ReqKeyName req;
    kv::GetKeyNameResponse res;
    req.set_keyrex(key);
    grpc::ClientContext context;
    grpc::Status status = stub_->GetKeyName(&context, req, &res);
    if (!status.ok()) {
        // TODO: log
        return MiniKV_GET_KEYNAME_FAIL;
    }
    auto p = res.val();
    for (auto it = p.begin(); it != p.end(); ++it) {
        ans.push_back(it->data());
    }
    return MiniKV_GET_KEYNAME_SUCCESS;
}