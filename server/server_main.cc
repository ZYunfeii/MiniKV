#include <iostream>
#include <signal.h>
#include "kvserver.h"

int main(int argc, char** argv) {
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    KVServer* kvServer = new KVServer("0.0.0.0", 6789);
    kvServer->serve();
    return 0;
}

