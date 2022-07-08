#ifndef _LOX_GHASH_H
#define _LOX_GHASH_H

#include "hash.h"
#include "char_vec.h"

#define ghash(t)                                                               \
    ({                                                                         \
        hasher h = hasher_new();                                               \
        ghash_to(&h, t);                                                       \
        hasher_finish(h);                                                      \
    })

// clang-format off
#define ghash_to(h, t) \
    ({ \
        typeof(h) _h = (h); \
        typeof(t) _t = (t); \
        _Generic(_t, \
            int *: hash_int, \
            uint8_t *: hash_uint8_t, \
            uint16_t *: hash_uint16_t, \
            uint32_t *: hash_uint32_t, \
            uint64_t *: hash_uint64_t, \
            bool *: hash_bool, \
            long *: hash_long, \
            string *: string_hash \
        )(_h, _t); \
    })
// clang-format on

#endif
