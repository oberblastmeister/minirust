// see https://github.com/rust-lang/rustc-hash

#ifndef _LOX_HASH_H
#define _LOX_HASH_H

#include "bits.h"
#include "prelude.h"
#include "read_bytes.h"
#include <assert.h>
#include <string.h>

typedef struct {
    size_t hash;
} hasher;

typedef uint64_t hash;

static inline void hash_size_t(hasher *h, size_t i) {
    // exploits wrapping multiplication of FX_CONST
    // size_t is unsigned, and will always wrap when overflowed
    // https://stackoverflow.com/questions/18195715/why-is-unsigned-integer-overflow-defined-behavior-but-signed-integer-overflow-is
    // hopefully the compiler will optimize this
    if (sizeof(size_t) == 8) {
        h->hash = (rotl_size_t(h->hash, 5) ^ i) * 0x517cc1b727220a95;
    } else {
        h->hash = (rotl_size_t(h->hash, 5) ^ i) * 0x9e3779b9;
    }
}

#define _MAKE_INT_HASHER(T)                                                    \
    static inline void hash_##T(hasher *h, T i) { hash_size_t(h, (size_t)i); }

_MAKE_INT_HASHER(uint8_t)
_MAKE_INT_HASHER(uint16_t)
_MAKE_INT_HASHER(uint32_t)
_MAKE_INT_HASHER(uint64_t)

#undef INT_HASHER

static inline void hash_bytes(hasher *h, uint8_t *bytes, size_t len) {
    while (len >= sizeof(size_t)) {
        hash_size_t(h, read_size_t(bytes));
        bytes += sizeof(size_t);
        len -= sizeof(size_t);
    }
    if ((sizeof(size_t) > 4) && len >= 4) {
        hash_size_t(h, (size_t)read_uint32_t(bytes));
        bytes += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    if ((sizeof(size_t) > 2) && len >= 2) {
        hash_size_t(h, (size_t)read_uint16_t(bytes));
        bytes += sizeof(uint16_t);
        len -= sizeof(uint16_t);
    }
    if ((sizeof(size_t) > 1) && len >= 1) {
        hash_size_t(h, (size_t)bytes[0]);
    }
}

static inline void hash_bool(hasher *h, bool b) { hash_uint8_t(h, (uint8_t)b); }

static inline hash hasher_finish(hasher h) { return (uint64_t)h.hash; }

#endif
