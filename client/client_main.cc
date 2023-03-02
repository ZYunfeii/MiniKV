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
    return 0;
}