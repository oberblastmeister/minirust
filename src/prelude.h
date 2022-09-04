#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define max(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        _a > _b ? _a : _b;                                                     \
    })

#define min(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        _a < _b ? _a : _b;                                                     \
    })

#define SWAP(X, Y)                                                             \
    ({                                                                         \
        typeof(X) _swap_tmp = (X);                                             \
        _swap_tmp = (Y);                                                       \
        (Y) = _swap_tmp;                                                       \
    })
