#include "gflags/gflags.h"
#include "../client/kvclient.h"

DEFINE_string(ip, "localhost", "ip for admin node");
DEFINE_int32(port, 6789, "port for admin node");
DEFINE_string(key, "", "key");
DEFINE_string(value, "", "value");
DEFINE_string(encoding, "string", "encoding, see in encoding.h");
DEFINE_string(operate, "set", "operate, e.g. set, del..");
DEFINE_uint64(expires, 5000, "expire time");
DEFINE_string(keyRex, "", "regex for key name");


DECLARE_string(ip);
DECLARE_int32(port);
DECLARE_string(key);
DECLARE_string(value);
DECLARE_string(encoding);
DECLARE_string(operate);
DECLARE_uint64(expires);
DECLARE_string(keyRex);

#include <unordered_map>
std::unordered_map<std::string, int> oper {
    {"set", KV_SET},
    {"del", KV_DEL},
    {"get", KV_GET},
    {"expire", KV_EXPIRE},
    {"findkey", KV_KEY_FIND}
};

std::unordered_map<std::string, int> enMap {
    {"string", MiniKV_STRING},
    {"list", MiniKV_LIST},
};

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    KVClient* kvclient = new KVClient(FLAGS_ip, FLAGS_port);
    std::string encoding = FLAGS_encoding;
    std::string operation  = FLAGS_operate;
    std::string key = FLAGS_key;
    std::string val = FLAGS_value;
    std::string keyRex = FLAGS_keyRex;
    uint64_t expires = FLAGS_expires;
    
    switch (oper[operation]) {
        case KV_SET : {
            kvclient->setKV(key, val, enMap[encoding]);
            break;
        }
        case KV_DEL : {
            kvclient->delK(key);
            break;
        }
        case KV_GET : {
            std::vector<std::string> ans;
            kvclient->getK(key, ans);
            for (int i = 0; i < ans.size(); ++i) {
                std::cout << ans[i] << std::endl;
            } 
            break;
        }
        case KV_EXPIRE : {
            kvclient->setExpires(key, expires);
            break;
        }
        case KV_KEY_FIND : {
            std::vector<std::string> ans;
            kvclient->getKeyName(keyRex, ans);
            for (int i = 0; i < ans.size(); ++i) {
                std::cout << ans[i] << std::endl;
            } 
            break;
        }
    }

    return 0;
}