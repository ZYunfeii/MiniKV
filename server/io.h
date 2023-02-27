#ifndef IO_H
#define IO_H
#include <iostream>

class KVio {
public:
    KVio(std::string fileName) {
        fp_ = fopen(fileName.data(), "w");
    }
    size_t kvioFileWrite(const void* buf, std::size_t len) {
        size_t retval;
        while (len) {
            retval = fwrite(buf, len, 1, fp_);
            len -= retval;
            buf = (char*)buf + retval;
        }
        return len;
    }
    FILE* fp_;
};

#endif