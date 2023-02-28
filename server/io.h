#ifndef IO_H
#define IO_H
#include <iostream>

class KVio {
public:
    KVio(std::string fileName) : fileName_(fileName){
        fp_ = fopen(fileName_.data(), "w+");
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
    size_t kvioFileRead(void* buf, std::size_t len) {
        size_t retval;
        while (len) {
            retval = fread(buf, 1, len, fp_);
            len -= retval;
            buf = (char*)buf + retval;
        }
        return len;
    }
    void clearFile() {
        fp_ = fopen(fileName_.data(), "w+");
    }
    inline void flushCache() {
        fflush(fp_);
    }
    inline void fseekTop() {
        fseek(fp_, 0, SEEK_SET);
    }
    FILE* fp_;
    std::string fileName_;
    ~KVio() {
        flushCache();
        fclose(fp_);
    }
};

#endif