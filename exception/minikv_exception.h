#ifndef MINIKV_EXCEPTION_H
#define MINIKV_EXCEPTION_H

#include <exception>

class MiniKVLogicErr : public std::exception {
    const char* what() const throw() {
        return "logic error";
    }
};

#endif