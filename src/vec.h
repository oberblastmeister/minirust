// ah yes, module functors in c

#include "bits.h"
#include "macro_util.h"
#include "prelude.h"
#include "ptr.h"
#include <stdlib.h>
#include <string.h>

#ifndef VEC
#define VEC JOIN(VEC_TYPE, vec)
#endif

#ifndef VEC_TYPE
#error "VEC_TYPE undefined"
// do this just to get better support for clangd
// the file will already fail to compile
#define VEC_TYPE int
typedef struct {
    size_t len;
    size_t cap;
    VEC_TYPE *data;
} VEC;
#endif

#ifdef VEC_STATIC
#define is_static static
#else
#define is_static
#endif

static void JOIN(VEC, maybe_resize_)(VEC *vec) {
    if (vec->cap == 0) {
        vec->cap = 8;
        vec->data = malloc(sizeof(VEC_TYPE) * 8);
        return;
    }
    if (vec->cap == vec->len) {
        int old_cap = vec->cap;
        vec->cap = old_cap < 8 ? 8 : old_cap * 2;
        vec->data = (VEC_TYPE *)realloc(vec->data, sizeof(VEC_TYPE) * vec->cap);
    }
}

void JOIN(VEC, reserve)(VEC *vec, size_t i) {
    if (vec->len + i > vec->cap) {
        vec->cap =
            min((size_t)8, (size_t)next_power_of_2((uint64_t)vec->len + i));
        vec->data = (VEC_TYPE *)realloc(vec->data, sizeof(VEC_TYPE) * vec->cap);
    }
}

is_static VEC JOIN(VEC, from_ptr_copied)(VEC_TYPE *p, size_t len) {
    size_t cap = (size_t)next_power_of_2((uint64_t)len);
    VEC_TYPE *data = (VEC_TYPE *)malloc(sizeof(VEC_TYPE) * cap);
    memcpy(data, p, sizeof(VEC_TYPE) * len);
    return (VEC){
        .len = len,
        .cap = cap,
        .data = data,
    };
}

is_static void JOIN(VEC, init)(VEC *vec) {
    vec->len = 0;
    vec->cap = 0;
    vec->data = invalid_ptr(VEC_TYPE);
}

is_static VEC JOIN(VEC, new)(void) {
    return (VEC){.len = 0, .cap = 0, .data = invalid_ptr(VEC_TYPE)};
}

is_static void JOIN(VEC, push)(VEC *vec, VEC_TYPE x) {
    JOIN(VEC, maybe_resize_)(vec);
    vec->data[vec->len] = x;
    vec->len++;
}

is_static inline VEC_TYPE JOIN(VEC, last)(const VEC *vec) {
    return vec->data[vec->len - 1];
}

is_static VEC_TYPE JOIN(VEC, pop)(VEC *vec) {
    VEC_TYPE x = vec->data[vec->len - 1];
    vec->len--;
    return x;
}

is_static VEC_TYPE JOIN(VEC, index)(const VEC *vec, int i) {
    return vec->data[i];
}

is_static void JOIN(VEC, clear)(VEC *vec) {
#ifdef VEC_TYPE_FREE
    for (size_t i = 0; i < vec->len; i++) {
        VEC_TYPE_FREE(&vec->data[i]);
    }
#endif
    vec->len = 0;
}

is_static void JOIN(VEC, free)(VEC *vec) {
    JOIN(VEC, clear)(vec);
    if (vec->cap != 0) {
        free(vec->data);
        JOIN(VEC, init)(vec);
    }
}

is_static size_t JOIN(VEC, len)(const VEC *vec) { return vec->len; }

#ifdef VEC_TYPE_COPY
is_static VEC JOIN(VEC, copy)(const VEC *vec) {
    VEC_TYPE *data = (VEC_TYPE *)malloc(sizeof(VEC_TYPE) * vec->cap);
    for (size_t i = 0; i < vec->len; i++) {
        data[i] = VEC_TYPE_COPY(&vec->data[i]);
    }
    return (VEC){.len = vec->len, .cap = vec->cap, .data = data};
}
#else
is_static VEC JOIN(VEC, copy)(const VEC *vec) {
    VEC_TYPE *data = malloc(sizeof(VEC_TYPE) * vec->cap);
    memcpy(data, vec->data, sizeof(VEC_TYPE) * vec->len);
    return (VEC){.len = vec->len, .cap = vec->cap, .data = data};
}
#endif

is_static VEC_TYPE *JOIN(VEC, ptr_at)(VEC *vec, int i) { return &vec->data[i]; }

is_static VEC_TYPE *JOIN(VEC, alloc)(VEC *vec, VEC_TYPE t) {
    JOIN(VEC, push)(vec, t);
    return &vec->data[vec->len - 1];
}

is_static bool JOIN(VEC, eq)(VEC *v1, VEC *v2) {
    return v1->len == v2->len &&
           memcmp(v1->data, v2->data, sizeof(VEC_TYPE) * v1->len) == 0;
}

#ifndef VEC_EXTEND
#undef is_static
#undef VEC
#undef VEC_STATIC
#undef VEC_TYPE
#undef VEC_TYPE_FREE
#undef VEC_TYPE_COPY
#endif
