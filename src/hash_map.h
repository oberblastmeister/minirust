#include "macro_util.h"
#include "prelude.h"

#ifndef HM_NAME
#define HM JOIN(JOIN(HM_VALUE, HM_KEY), map)
#else
#define HM HM_NAME
#endif

#define HM_BUCKET JOIN(JOIN(HM_VALUE, HM_KEY), bucket)

#ifndef HM_KEY
#error "HM_KEY undefined"
// do this just to get better support for clangd
// the file will already fail to compile
#define HM_KEY int
#endif

#ifndef HM_VALUE
#error "HM_VALUE undefined"
#define HM_VALUE int
typedef struct {
    int hash;
    HM_KEY key;
    HM_VALUE value;
} HM_BUCKET;

typedef struct {
    size_t len;
    size_t cap;
    HM_BUCKET *data;
} VEC;
#endif

#ifndef HM_KEY_HASH
#error "HM_KEY_HASH undefined"
#define HM_KEY_HASH JOIN(HM_KEY, hash)
int HM_KEY_HASH(int i) { return i; }
#endif

#if !defined(HM_KEY_FREE) && !defined(HM_VALUE_FREE)
#define _HM_FREE_POD
#endif

#if !defined(HM_KEY_COPY) && !defined(HM_VALUE_COPY)
#define _HM_COPY_POD
#endif

static inline void JOIN(HM_BUCKET, free)(HM_BUCKET *bucket) {
#ifdef HM_KEY_FREE
    HM_KEY_FREE(&bucket->key);
#endif
#ifdef HM_VALUE_FREE
    HM_VALUE_FREE(&bucket->key);
#endif
}

static inline HM_BUCKET JOIN(HM_BUCKET, copy)(HM_BUCKET *bucket) {
    HM_BUCKET new_bucket = {.hash = bucket->hash};
#ifdef HM_KEY_COPY
    new_bucket.key = HM_KEY_COPY(&bucket->key);
#else
    new_bucket.key = bucket->key;
#endif
#ifdef HM_VALUE_COPY
    HM_VALUE_COPY(&bucket->value);
#else
    new_bucket.value = bucket->value;
#endif
    return new_bucket;
}

#ifndef HM_EXTEND
#undef _HM_FREE_POD
#undef _HM_COPY_POD
#undef HM_NAME
#undef HM
#undef HM_BUCKET
#undef HM_KEY
#undef HM_VALUE
#undef HM_KEY_FREE
#undef HM_KEY_COPY
#undef HM_VALUE_FREE
#undef HM_VALUE_COPY
#undef HM_KEY_HASH
#endif
