#ifndef T
#error "Template type T undefined for <vec.h>"
// do this just to get better ide support
// the file will already fail to compile
#define T int
#endif

#include "macro_util.h"
#include "prelude.h"
#include <stdlib.h>

#ifdef S
#define is_static static
#else
#define is_static
#endif

#define V JOIN(T, vec)

#ifndef VEC_DEFINED
typedef struct {
    size_t len;
    size_t cap;
    T *data;
} V;
#else
#undef VEC_DEFINED
#endif

static void JOIN(V, maybe_resize_)(V *vec) {
    if (vec->cap == vec->len) {
        int old_cap = vec->cap;
        vec->cap = old_cap < 8 ? 8 : old_cap * 2;
        vec->data = realloc(vec->data, sizeof(T) * vec->cap);
    }
}

is_static void JOIN(V, init)(V *vec) {
    vec->len = 0;
    vec->cap = 0;
    vec->data = NULL;
}

is_static V JOIN(V, new)(void) {
    V vec;
    JOIN(V, init)(&vec);
    return vec;
}

is_static void JOIN(V, push)(V *vec, T x) {
    JOIN(V, maybe_resize_)(vec);
    vec->data[vec->len] = x;
    vec->len++;
}

is_static inline T JOIN(V, last)(V *vec) { return vec->data[vec->len - 1]; }

is_static T JOIN(V, pop)(V *vec) {
    T x = JOIN(V, last)(vec);
    vec->len--;
    return x;
}

is_static T JOIN(V, index)(V *vec, int i) { return vec->data[i]; }

is_static void JOIN(V, free)(V *vec) {
    free(vec->data);
    JOIN(V, init)(vec);
}

is_static size_t JOIN(V, len)(V *vec) { return vec->len; }

#undef is_static
#undef V

// Hold preserves `T` if other containers wish to extend `vec.h`.
#ifdef HOLD
#undef HOLD
#else
#undef T
#undef S
#endif
