#ifndef KV_Client_H
#define KV_Client_H

#include <string>
#include <grpcpp/grpcpp.h>
#include "../proto/kvserver.grpc.pb.h"
#include "../type/encoding.h"

typedef struct GetRes {
    uint32_t encoding;
    std::vector<std::string> data;
} getRes;

class KVClient {
public:
    KVClient(std::string ip, uint32_t port);

    int setKV(std::string key, std::string val, uint32_t encoding);
    GetRes getK(std::string key);
private:
    std::unique_ptr<kv::KVServer::Stub> stub_;
    std::string ip_;
    uint32_t port_;
};


#endif