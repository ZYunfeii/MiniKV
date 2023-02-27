#include <iostream>
#include "kvserver.h"

int main(int argc, char** argv) {
    KVServer* kvServer = new KVServer("0.0.0.0", 6789);
    kvServer->serve();
    return 0;
}

