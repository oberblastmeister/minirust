#ifndef _LOX_GEQ_H
#define _LOX_GEQ_H

#include "prelude.h"
#include "types_list_x.h"
#include "char_vec.h"

#define _MAKE_SIMPLE_EQ(T)                                                     \
    static inline bool T##_eq(const T *x, const T *y) { return *x == *y; }

#define X _MAKE_SIMPLE_EQ
INT_LIKE_LIST_X
#undef X

// clang-format off
#define geq(x, y) \
    ({ \
        typeof(x) _x = (x); \
        typeof(y) _y = (y); \
        _Generic(_x, \
            int *: int_eq, \
            uint8_t *: uint8_t_eq, \
            uint16_t *: uint16_t_eq, \
            uint32_t *: uint32_t_eq, \
            uint64_t *: uint64_t_eq, \
            string *: string_eq \
        )(_x, _y); \
    })
// clang-format on

#endif
