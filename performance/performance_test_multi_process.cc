#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <vector>

#include <sys/types.h> 
#include <sys/wait.h>
#include "../client/kvclient.h"
#define BUF_SIZE 512
class ReadShareMem {
public:
    int shmid;
    key_t key;
    char* shmadd;
};

int main(int argc, char** argv) {
    int testTime = 5;
    int processNum = 10;
    std::vector<ReadShareMem> smVec;
    for (int i = 1; i <= processNum; ++i) {
        ReadShareMem sm;
        sm.key = ftok("./", 100 + i);
        if (sm.key < 0) {
            perror( "ftok error\n");
            exit(1);
        }
        sm.shmid = shmget(sm.key, BUF_SIZE, IPC_CREAT | 0666);
        sm.shmadd = (char*)shmat(sm.shmid, NULL, 0);
        smVec.emplace_back(sm);
    }
    // system("ipcs -m");
    for (int i = 1; i <= processNum; ++i) {
        pid_t p = fork();
        if (p == 0) {
           if (execl("./child_process", std::to_string(testTime).data(), std::to_string(i).data(), NULL) == -1) {
                perror( "execl error\n");
                exit(1);
           }
        } else if (p > 0) {
            
        } else {
            perror("error in fork!\n"); 
            exit(1);
        }
    }
    for (int i = 0; i < processNum; ++i) {
        auto pid = wait(NULL);
        if (pid == -1) {
            perror( "wait error\n");
        } else {
            printf("pid %d exit!\n", pid); 
        }
    }
    int res = 0;
    for (int i = 0; i < processNum; ++i) {
        int count = *(reinterpret_cast<int*>(smVec[i].shmadd));
        res += count;
        shmdt(smVec[i].shmadd);
        shmctl(smVec[i].shmid, IPC_RMID, NULL);
    }
    fprintf(stdout, "TPS: %d\n", res / testTime);
    exit(0);
}