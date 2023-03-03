#include "gflags/gflags.h"
#include "../client/kvclient.h"
#include "draw_table.h"

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

std::unordered_map<int, std::string> enStatus;
void initResponseEncoding() {
    enStatus[MiniKV_SET_SUCCESS] = "OK";
    enStatus[MiniKV_GET_SUCCESS] = "OK";
    enStatus[MiniKV_DEL_SUCCESS] = "OK";
    enStatus[MiniKV_SET_EXPIRE_SUCCESS] = "OK";
    enStatus[MiniKV_GET_KEYNAME_SUCCESS] = "OK";
    enStatus[MiniKV_SET_FAIL] = "Fail!";
    enStatus[MiniKV_GET_FAIL] = "Fail!";
    enStatus[MiniKV_DEL_FAIL] = "Fail!";
    enStatus[MiniKV_SET_EXPIRE_FAIL] = "Fail!";
    enStatus[MiniKV_GET_KEYNAME_FAIL] = "Fail!";
}

void drawStatus(int ans) {
    std::vector<std::vector<std::string>> data;
    data.push_back({enStatus[ans]});
    std::vector<int> max;
    maxLenForEveryCol(data, max);
    std::vector<std::string> head = {"Status"};
    drawDatas(max, data, head, 1, 1);
}

void drawMultiEleOneCol(std::vector<std::string>& ans, std::string headName) {
    std::vector<std::vector<std::string>> data;
    for (int i = 0; i < ans.size(); ++i) {
        data.push_back({ans[i]});
    }
    std::vector<int> max;
    maxLenForEveryCol(data, max);
    std::vector<std::string> head = {headName};
    drawDatas(max, data, head, 1, ans.size());
}

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    KVClient* kvclient = new KVClient(FLAGS_ip, FLAGS_port);
    std::string encoding = FLAGS_encoding;
    std::string operation  = FLAGS_operate;
    std::string key = FLAGS_key;
    std::string val = FLAGS_value;
    std::string keyRex = FLAGS_keyRex;
    uint64_t expires = FLAGS_expires;
    initResponseEncoding();
    switch (oper[operation]) {
        case KV_SET : {
            int ans = kvclient->setKV(key, val, enMap[encoding]);
            drawStatus(ans);
            break;
        }
        case KV_DEL : {
            int ans = kvclient->delK(key);
            drawStatus(ans);
            break;
        }
        case KV_GET : {
            std::vector<std::string> ans;
            kvclient->getK(key, ans);
            drawMultiEleOneCol(ans, "Value");
            break;
        }
        case KV_EXPIRE : {
            int ans = kvclient->setExpires(key, expires);
            drawStatus(ans);
            break;
        }
        case KV_KEY_FIND : {
            std::vector<std::string> ans;
            kvclient->getKeyName(keyRex, ans);
            drawMultiEleOneCol(ans, "Key");
            break;
        }
    }

    return 0;
}