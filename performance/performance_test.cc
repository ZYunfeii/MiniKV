#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include "../client/kvclient.h"
using namespace std;
#define TEST_TIME 5 // s
condition_variable cond;
mutex mtx;

void requestThread(int* count) {
    KVClient* c = new KVClient("127.0.0.1", 6789);
    {
        unique_lock<mutex> lk(mtx);
        cond.wait(lk);
    }
    auto start = std::chrono::system_clock::now();
    auto startTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    vector<string> ans;
    while (1) {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        if (timestamp - startTimeStamp >= TEST_TIME * 1000) break;
        // int res = c->setKV("yunfei", "23", MiniKV_STRING);
        int res = c->getK("yunfei", ans);
        if (res == MiniKV_SET_SUCCESS || res == MiniKV_GET_SUCCESS) {
            (*count)++;
        } else {
            fprintf(stderr, "set key failed!\n");
            exit(1);
        }
    }
}

int main(int argc, char** argv) {
    int threadNum = 20;
    vector<int> countVec(threadNum, 0);
    vector<thread> threadVec(threadNum);
    for (int i = 0; i < threadNum; ++i) {
        threadVec.emplace_back(thread(requestThread, &countVec[i]));
    }
    
    sleep(1);
    cond.notify_all();
    // for (int i = 0; i < threadNum; ++i) {
    //     threadVec[i].join();
    // }
    sleep(TEST_TIME + 1);
    int res = 0;
    for (int i = 0; i < threadNum; ++i) {
        res += countVec[i];
    }
    fprintf(stdout, "TPS: %d\n", res / TEST_TIME);
    exit(0);
}
