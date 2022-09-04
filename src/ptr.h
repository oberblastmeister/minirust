#pragma once

#include <stdalign.h>
#include <string.h>

// returns an invalid pointer that is not null and aligned
// useful for null pointer optimizations
#define invalid_ptr(T) (T *)alignof(T)

#define is_none_pattern(P, T)                                                  \
    ({                                                                         \
        T t = new_none_pattern(T);                                             \
        memcmp(P, &t, sizeof(T)) == 0;                                         \
    })

#define new_none_pattern(T)                                                    \
    ({                                                                         \
        T t;                                                                   \
        memset(&t, 0, sizeof(T));                                              \
        t;                                                                     \
    })
