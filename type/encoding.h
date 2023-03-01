#ifndef ENCODING_H
#define ENCODING_H

enum {
    MiniKV_STRING = 1,
    MiniKV_LIST,
    MiniKV_SET_SUCCESS,
    MiniKV_SET_FAIL,
    MiniKV_GET_SUCCESS,
    MiniKV_GET_FAIL,
    MiniKV_DEL_FAIL,
    MiniKV_DEL_SUCCESS,
    MiniKV_SET_EXPIRE_SUCCESS,
    MiniKV_SET_EXPIRE_FAIL
};

#endif