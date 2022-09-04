#pragma once

#include "prelude.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

/**
 * @return The next highest power of two, or the same number if a power of two
 * is given. 0 is always returned if given for i.
 */
static inline uint64_t next_power_of_2(uint64_t i) {
    i--;
    i |= i >> 1;
    i |= i >> 2;
    i |= i >> 4;
    i |= i >> 8;
    i |= i >> 16;
    i |= i >> 32;
    i++;
    return i;
}

// see
// https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
#define MAKE_ROT(T)                                                            \
    static inline T rotl_##T(T n, uint32_t c) {                                \
        const uint32_t mask = (CHAR_BIT * sizeof(n) - 1);                      \
        c &= mask;                                                             \
        return (n << c) | (n >> ((-c) & mask));                                \
    }                                                                          \
    static inline T rotr_##T(T n, uint32_t c) {                                \
        const uint32_t mask = (CHAR_BIT * sizeof(n) - 1);                      \
        c &= mask;                                                             \
        return (n >> c) | (n << ((-c) & mask));                                \
    }

MAKE_ROT(size_t)
MAKE_ROT(uint8_t)
MAKE_ROT(uint16_t)
MAKE_ROT(uint32_t)
MAKE_ROT(uint64_t)

#undef MAKE_ROT

#define rotl(n, c)                                                             \
    ({                                                                         \
        typeof(n) _n = (n);                                                    \
        typeof(c) _c = (c);                                                    \
        _Generic((_n), uint8_t                                                 \
                 : rotl8(_n, _c), uint16_t                                     \
                 : rotl16(_n, _c), uint32_t                                    \
                 : rotl32(_n, _c), uint64_t                                    \
                 : rotl64(_n, _c));                                            \
    })

#define rotr(n, c)                                                             \
    ({                                                                         \
        typeof(n) _n = (n);                                                    \
        typeof(c) _c = (c);                                                    \
        _Generic((n), uint8_t                                                  \
                 : rotr8(_n, _c), uint16_t                                     \
                 : rotr16(_n, _c), uint32_t                                    \
                 : rotr32(_n, _c), uint64_t                                    \
                 : rotr64(_n, _c));                                            \
    })
