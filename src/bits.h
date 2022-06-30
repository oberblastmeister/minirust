#ifndef _LOX_BITS_H
#define _LOX_BITS_H

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
#define MAKE_ROT(T, NAME)                                                      \
    static inline T rotl##NAME(T n, uint32_t c) {                              \
        const uint32_t mask = (CHAR_BIT * sizeof(n) - 1);                      \
        c &= mask;                                                             \
        return (n << c) | (n >> ((-c) & mask));                                \
    }                                                                          \
    static inline T rotr##NAME(T n, uint32_t c) {                              \
        const uint32_t mask = (CHAR_BIT * sizeof(n) - 1);                      \
        c &= mask;                                                             \
        return (n >> c) | (n << ((-c) & mask));                                \
    }

MAKE_ROT(uint8_t, 8)
MAKE_ROT(uint16_t, 16)
MAKE_ROT(uint32_t, 32)
MAKE_ROT(uint64_t, 64)

#undef MAKE_ROT
    
// #define rotl(n, c) \
//     _Generic((n), \
//         uint8_t: rotl8((n), (s))
//     )

#endif
