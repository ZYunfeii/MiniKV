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
            retval = fwrite(buf, 1, len, fp_);
            len -= retval;
            buf = (char*)buf + retval;
        }
        return len;
    }
    void flushCache() {
        fflush(fp_);
    }
    FILE* fp_;
    ~KVio() {
        fclose(fp_);
    }
};

#endif