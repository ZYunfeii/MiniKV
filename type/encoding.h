#ifndef ENCODING_H
#define ENCODING_H


#define MiniKV_STRING (1<<0)
#define MiniKV_LIST (1<<1)

#define MiniKV_SET_SUCCESS (1<<2)
#define MiniKV_SET_FAIL (1<<3)
#define MiniKV_GET_SUCCESS (1<<4)
#define MiniKV_GET_FAIL (1<<5)
#define MiniKV_DEL_FAIL (1<<6)
#define MiniKV_DEL_SUCCESS (1<<7)

#endif