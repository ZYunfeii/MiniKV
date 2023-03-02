#include <iostream>
#include "kvclient.h"
#define DEBUG

#ifdef DEBUG
#include "test.h"
#endif


int main(int argc, char** argv) {
    KVClient* kvclient = new KVClient("127.0.0.1", 6789);
    // testExpire(kvclient);
    testSetLists(kvclient);
    


    // res = kvclient->setKV("qjx", "23", MiniKV_LIST);
    // if (res == MiniKV_SET_SUCCESS) {
    //     fprintf(stdout, "set key success!\n");
    // } else {
    //     fprintf(stderr, "set key failed!\n");
    //     exit(1);
    // }
    // res = kvclient->setKV("qjx", "24", MiniKV_LIST);
    // if (res == MiniKV_SET_SUCCESS) {
    //     fprintf(stdout, "set key success!\n");
    // } else {
    //     fprintf(stderr, "set key failed!\n");
    //     exit(1);
    // }
    // ans = kvclient->getK("qjx");
    // if (ans.encoding == MiniKV_GET_SUCCESS) {
    //     fprintf(stdout, "get key success!\n");
    // } else {
    //     fprintf(stderr, "get key failed!\n");
    //     exit(1);
    // }
    // for (int i = 0; i < ans.data.size(); ++i) {
    //     std::cout << ans.data[i] << " ";
    // } std::cout << std::endl;
    return 0;
}