// see https://github.com/rust-lang/rustc-hash

#pragma once

#include "bits.h"
#include "prelude.h"
#include "read_bytes.h"
#include "types_list_x.h"
#include <assert.h>
#include <string.h>

typedef struct {
    size_t hash;
} hasher;

typedef size_t hash;

static inline void hash_size_t(hasher *h, const size_t *i) {
    // exploits wrapping multiplication of FX_CONST
    // size_t is unsigned, and will always wrap when overflowed
    // https://stackoverflow.com/questions/18195715/why-is-unsigned-integer-overflow-defined-behavior-but-signed-integer-overflow-is
    // hopefully the compiler will optimize this
    if (sizeof(size_t) == 8) {
        h->hash = (rotl_size_t(h->hash, 5) ^ *i) * 0x517cc1b727220a95;
    } else {
        h->hash = (rotl_size_t(h->hash, 5) ^ *i) * 0x9e3779b9;
    }
}

#define _MAKE_INT_HASHER(T)                                                    \
    static inline void hash_##T(hasher *h, const T *p) {                       \
        size_t i = (size_t)*p;                                                 \
        hash_size_t(h, &i);                                                    \
    }

#define X _MAKE_INT_HASHER
INT_LIKE_LIST_X
#undef X

#undef _INT_LIST_X
#undef INT_HASHER

static inline void hash_bytes(hasher *h, const char *bytes, size_t len) {
    while (len >= sizeof(size_t)) {
        size_t i = read_size_t(bytes);
        hash_size_t(h, &i);
        bytes += sizeof(size_t);
        len -= sizeof(size_t);
    }
    if ((sizeof(size_t) > 4) && len >= 4) {
        size_t i = (size_t)read_uint32_t(bytes);
        hash_size_t(h, &i);
        bytes += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    if ((sizeof(size_t) > 2) && len >= 2) {
        size_t i = (size_t)read_uint16_t(bytes);
        hash_size_t(h, &i);
        bytes += sizeof(uint16_t);
        len -= sizeof(uint16_t);
    }
    if ((sizeof(size_t) > 1) && len >= 1) {
        size_t i = (size_t)bytes[0];
        hash_size_t(h, &i);
    }
}

static inline hasher hasher_new(void) { return (hasher){.hash = 0}; }

static inline hash hasher_finish(hasher h) { return (hash)h.hash; }
