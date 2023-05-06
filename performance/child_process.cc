#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <vector>
#include "../client/kvclient.h"
using namespace std;
#define BUF_SIZE 512
int main(int argc, char** argv) {
    printf("child process start\n");
    if (argc < 2) {
        cout << "input num incorrect!" << endl;
        exit(1);
    }
    int testTime = stoi(string(argv[0], argv[0] + strlen(argv[0])));
    int pid = stoi(string(argv[1], argv[1] + strlen(argv[1])));
    printf("child process receive testTime: %d\n", testTime);
    int count = 0;
    KVClient* c = new KVClient("127.0.0.1", 6789);
    auto start = std::chrono::system_clock::now();
    auto startTimeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    vector<string> ans;
    while (1) {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        if (timestamp - startTimeStamp >= testTime * 1000) break;
        // int res = c->setKV("yunfei", "23", MiniKV_STRING);
        int res = c->getK("yunfei", ans);
        if (res == MiniKV_SET_SUCCESS || res == MiniKV_GET_SUCCESS) {
            count++;
        } else {
            fprintf(stderr, "set key failed!\n");
            exit(1);
        }
    }
    // write to the shared memory, default 10 child process.
    int shmid;
    int ret;
    key_t key;
    char* shmadd;

    key = ftok("./", 100 + pid);
    shmid = shmget(key, BUF_SIZE, IPC_CREAT | 0666);
    shmadd = (char*)shmat(shmid, NULL, 0);
    bzero(shmadd, BUF_SIZE);
    memcpy(shmadd, &count, sizeof(count));


    exit(0);
}