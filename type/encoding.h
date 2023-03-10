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
    MiniKV_SET_EXPIRE_FAIL,
    MiniKV_KEY_EMPTY,
    MiniKV_GET_KEYNAME_FAIL,
    MiniKV_GET_KEYNAME_SUCCESS
};

enum {
    KV_SET = 100,
    KV_DEL,
    KV_GET,
    KV_EXPIRE,
    KV_KEY_FIND
};

#endif