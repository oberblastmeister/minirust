#ifndef _LOX_MEMORY_H
#define _LOX_MEMORY_H

#include <stdlib.h>

void *reallocate(void *ptr, size_t size);

void cleanup_free(void *p);

#define autofree __attribute__((__cleanup__(cleanup_free)))

#define cleanup(f) __attribute__((__cleanup__(f)))

#define CLEANUP_FUNC(type, func)                                               \
    static inline void func##p(type *p) {                                      \
        if (*p)                                                                \
            func(*p);                                                          \
    }                                                                          \
    struct __useless_struct_to_allow_trailing_semicolon__

#endif
