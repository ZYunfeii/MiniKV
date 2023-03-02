#ifndef TEST_H
#define TEST_H
#include "kvclient.h"

void testExpire(KVClient* c) {
    int res = c->setKV("yunfei", "23", MiniKV_STRING);
    if (res == MiniKV_SET_SUCCESS) {
        fprintf(stdout, "set key success!\n");
    } else {
        fprintf(stderr, "set key failed!\n");
        exit(1);
    }
    std::vector<std::string> ans;
    c->getK("yunfei", ans);
    std::cout << ans[0] << std::endl;
    res = c->setExpires("yunfei", 1000);
    if (res == MiniKV_SET_EXPIRE_SUCCESS) {
        fprintf(stdout, "set expire success!\n");
    } else {
        fprintf(stderr, "set expire failed!\n");
        exit(1);
    }
    sleep(3);
    c->getK("yunfei", ans);
    std::cout << (ans.empty() ? "expired" : "expire doesn't work") << std::endl;
}

void testSetLists(KVClient* c) {
    for (int i = 0; i < 100; ++i) {
        int res = c->setKV("yunfei", "23", MiniKV_LIST);
        if (res == MiniKV_SET_SUCCESS) {
            fprintf(stdout, "set key success!\n");
        } else {
            fprintf(stderr, "set key failed!\n");
            exit(1);
        }
    }
    std::vector<std::string> ans;
    c->getK("yunfei", ans);
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i] << " ";
    } std::cout << std::endl;
}

#endif