#ifndef _LOX_BITS_H
#define _LOX_BITS_H

#include "prelude.h"

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

#endif
