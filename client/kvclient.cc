#include "kvclient.h"

KVClient::KVClient(std::string ip, uint32_t port) : ip_(ip), port_(port) {
    std::string serverAddr = ip_ + ":" + std::to_string(port_);
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(serverAddr, grpc::InsecureChannelCredentials());
    stub_ = kv::KVServer::NewStub(channel);
}

int KVClient::setKV(std::string key, std::string val, uint32_t encoding) {
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

GetRes KVClient::getK(std::string key) {
    GetRes ans;
    kv::ReqK req;
    kv::GetKResponse res;
    req.set_key(key);
    grpc::ClientContext context;
    grpc::Status status = stub_->GetKV(&context, req, &res);
    if (!status.ok() || res.flag() == false) {
        // TODO: log
        ans.encoding = MiniKV_GET_FAIL;
        ans.data = {};
        return ans;
    }
    std::vector<std::string> data;
    ans.encoding = MiniKV_GET_SUCCESS;
    auto p = res.val();
    for (auto it = p.begin(); it != p.end(); ++it) {
        data.push_back(it->data());
    }
    ans.data = data;
    return ans;
}