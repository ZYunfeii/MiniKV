#ifndef KV_Client_H
#define KV_Client_H

#include <string>
#include <grpcpp/grpcpp.h>
#include "../proto/kvserver.grpc.pb.h"
#include "../type/encoding.h"

class KVClient {
public:
    KVClient(std::string ip, uint32_t port);

    int setKV(std::string key, std::string val, uint32_t encoding);
    int delK(std::string key);
    int setExpires(std::string key, uint64_t millisecond);
    int getK(std::string key, std::vector<std::string>& res);
    int getKeyName(std::string keyRegex, std::vector<std::string>& res);
    
private:
    std::unique_ptr<kv::KVServer::Stub> stub_;
    std::string ip_;
    uint32_t port_;
};


#endif