#include "hash.h"

// clang-format off
#define ghash(h, t) \
    ({ \
        typeof(h) _h = (h); \
        typeof(t) _t = (t); \
        _Generic(_t, \
            uint8_t: hash_uint8_t(_h, _t), \
            uint16_t: hash_uint16_t(_h, _t), \
            uint32_t: hash_uint32_t(_h, _t), \
            uint64_t: hash_uint64_t(_h, _t), \
            size_t: hash_size_t(_h, _t), \
            bool: hash_bool(_h, _t) \
        ); \
    })
// clang-format on
