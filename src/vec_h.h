
#ifndef T
#error "Template type T undefined for <vec.h>"
// do this just to get better ide support
// the file will already fail to compile
#define T int
#endif

#include "macro_util.h"
#include "prelude.h"
#include <stdlib.h>

#define V JOIN(T, vec)

typedef struct {
    size_t len;
    size_t cap;
    T *data;
} V;

#define VEC_DEFINED

void JOIN(V, init)(V *vec);

V JOIN(V, new)(void);

void JOIN(V, push)(V *vec, T x);

inline T JOIN(V, last)(V *vec);

T JOIN(V, pop)(V *vec);

T JOIN(V, index)(V *vec, int i);

void JOIN(V, free)(V *vec);

size_t JOIN(V, len)(V *vec);

#undef V

// Hold preserves `T` if other containers wish to extend `vec.h`.
#ifdef HOLD
#undef HOLD
#else
#undef T
#undef S
#endif
