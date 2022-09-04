#pragma once

#include "prelude.h"
#include <string.h>

#define _MAKE_READER(T)                                                        \
    static inline T read_##T(const char *bytes) {                                 \
        T t;                                                                   \
        memcpy(&t, bytes, sizeof(T));                                          \
        return t;                                                              \
    }

_MAKE_READER(size_t)
_MAKE_READER(uint16_t)
_MAKE_READER(uint32_t)
_MAKE_READER(uint64_t)

#undef _MAKE_READER
