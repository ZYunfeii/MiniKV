#ifndef IO_H
#define IO_H
#include <iostream>
#include "../log/log.h"
class KVio {
public:
    KVio(std::string fileName) : fileName_(fileName){
        fp_ = fopen(fileName_.data(), "r");
        if (!fp_) {
            kvlogi("rdb file doesn't exits.");
            return;
        }
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
            if (retval < 0) return -1;
            len -= retval;
            buf = (char*)buf + retval;
        }
        return len;
    }
    bool reachEOF() {
        return feof(fp_);
    }
    bool empty() {
        if (!fp_) return true;
        getc(fp_);
        if (feof(fp_)) {
            return true; // empty file
        }
        fseek(fp_,-1,SEEK_CUR);
        return false;
    }
    void clearFile() {
        fp_ = fopen(fileName_.data(), "w+"); // "w+" will clear the file or create the new file
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