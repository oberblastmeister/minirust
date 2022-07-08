#include "geq.h"
#include "ghash.h"
#include "hash.h"
#include "macro_util.h"
#include "prelude.h"
#include "ptr.h"
#include <stdio.h>
#include <stdlib.h>

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
static void dummy_int_free(int *i) {}
static int dummy_int_copy(int *i) { return *i; }
#define HM_KEY_FREE dummy_int_free
#define HM_KEY_COPY dummy_int_copy
#endif

#ifndef HM_VALUE
#error "HM_VALUE undefined"
#define HM_VALUE int
typedef struct {
    HM_KEY key;
    HM_VALUE value;
} HM_BUCKET;

typedef struct {
    size_t len;
    size_t cap;
    size_t threshold;
    hash *hashes;
    HM_BUCKET *data;
} HM;
#define HM_VALUE_FREE dummy_int_free;
#define HM_VALUE_COPY dummy_int_copy;
#endif

#if defined(HM_KEY_FREE) || defined(HM_VALUE_FREE)
#define _HM_FREE_COMPLEX
#endif

#if defined(HM_KEY_COPY) || defined(HM_VALUE_COPY)
#define _HM_COPY_COMPLEX
#endif

static inline void JOIN(HM_BUCKET, free)(HM_BUCKET *bucket) {
#ifdef HM_KEY_FREE
    HM_KEY_FREE(&bucket->key);
#endif
#ifdef HM_VALUE_FREE
    HM_VALUE_FREE(&bucket->value);
#endif
}

static inline HM_BUCKET JOIN(HM_BUCKET, copy)(HM_BUCKET *bucket) {
    HM_BUCKET new_bucket;
#ifdef HM_KEY_COPY
    new_bucket.key = HM_KEY_COPY(&bucket->key);
#else
    new_bucket.key = bucket->key;
#endif
#ifdef HM_VALUE_COPY
    new_bucket.value = HM_VALUE_COPY(&bucket->value);
#else
    new_bucket.value = bucket->value;
#endif
    return new_bucket;
}

HM JOIN(HM, new)() {
    return (HM){
        .len = 0,
        .cap = 0,
        .threshold = 0,
        .hashes = invalid_ptr(hash),
        .data = invalid_ptr(HM_BUCKET),
    };
}

void JOIN(HM, free)(HM *hm) {
    if (hm->cap == 0) {
        return;
    }

#ifdef _HM_FREE_COMPLEX
    for (size_t i = 0; i < hm->cap; i++) {
        if (hm->hashes[i] != 0) {
            JOIN(HM_BUCKET, free)(&hm->data[i]);
        }
    }
#endif

    free(hm->hashes);
    free(hm->data);
}

void _JOIN(HM, add_with_hash)(HM *hm, hash h, HM_KEY k, HM_VALUE v) {
    size_t mask = hm->cap - 1;
    size_t i = h & mask;
    size_t cap = hm->cap;
    int dist = 0;
    HM_BUCKET bucket = {.key = k, .value = v};
    while (true) {
        hash h_at = hm->hashes[i];
        if (h_at == 0) {
            hm->hashes[i] = h;
            hm->data[i] = bucket;
            return;
        }
        int dist_at = (i + cap - (h_at & mask)) & mask;
        if (dist_at < dist) {
            SWAP(h, hm->hashes[i]);
            SWAP(bucket, hm->data[i]);
            dist = dist_at;
        }
        i = (i + 1) & mask;
        dist++;
    }
}

void _JOIN(HM, resize)(HM *hm, size_t new_cap) {
    HM new_hm = {
        .hashes = (hash *)calloc(new_cap, sizeof(hash)),
        .data = (HM_BUCKET *)malloc(sizeof(HM_BUCKET) * new_cap),
        .threshold = (new_cap * 10 + 9) / 11,
        .cap = new_cap,
    };

    // hm->cap starts at 0, so no null dereference when lazy initialization
    for (size_t i = 0; i < hm->cap; i++) {
        if (hm->hashes[i] != 0) {
            _JOIN(HM, add_with_hash)
            (&new_hm, hm->hashes[i], hm->data[i].key, hm->data[i].value);
        }
    }

    free(hm->hashes);
    free(hm->data);

    *hm = new_hm;
}

void _JOIN(HM, maybe_resize)(HM *hm) {
    // >= is important because len and threshold both start at 0 at creation
    if (hm->len >= hm->threshold) {
        _JOIN(HM, resize)(hm, min((size_t)8, hm->cap * 2));
    }
}

/**
 * @return Returns true if a previous value was evicted.
 */
bool JOIN(HM, insert)(HM *hm, HM_KEY k, HM_VALUE v) {
    _JOIN(HM, maybe_resize)(hm);

    size_t mask = hm->cap - 1;
    hash h = ghash(&k);
    size_t i = h & mask;
    size_t cap = hm->cap;
    int dist = 0;
    HM_BUCKET bucket = {.key = k, .value = v};
    while (true) {
        hash h_at = hm->hashes[i];
        if (h_at == 0) {
            hm->hashes[i] = h;
            hm->data[i] = bucket;
            return false;
        }
        if (h_at == h && geq(&hm->data[i].key, &bucket.key)) {
            hm->data[i].value = bucket.value;
            return true;
        }
        int dist_at = (i + cap - (h_at & mask)) & mask;
        if (dist_at < dist) {
            SWAP(h, hm->hashes[i]);
            SWAP(bucket, hm->data[i]);
            dist = dist_at;
        }
        i = (i + 1) & mask;
        dist++;
    }
}

static inline size_t _JOIN(HM, get_index)(HM *hm, const HM_KEY *k) {
    // make sure that we don't do null dereference because of lazy
    // initialization
    if (hm->cap == 0) {
        return (size_t)-1;
    }

    size_t cap = hm->cap;
    size_t mask = cap - 1;
    hash h = ghash((HM_KEY *)k);
    size_t i = h & mask;
    int dist = 0;
    while (true) {
        hash h_at = hm->hashes[i];
        if (h_at == 0) {
            return (size_t)-1;
        }
        int dist_at = (i + cap - (h_at & mask)) & mask;
        if (dist > dist_at) {
            return (size_t)-1;
        }
        if (h_at == h && geq(&hm->data[i].key, k)) {
            return i;
        }
        i = (i + 1) & mask;
        dist++;
    }
}

/**
 * @return Returns NULL if k could not be found. Otherwise, returns the pointer
 * to the associated value.
 */
HM_VALUE *JOIN(HM, get_ptr)(HM *hm, const HM_KEY *k) {
    size_t i = _JOIN(HM, get_index)(hm, k);
    if (i == (size_t)-1) {
        return NULL;
    } else {
        return &hm->data[i].value;
    }
}

/**
 * @return Returns true if k is in the map.
 */
bool JOIN(HM, contains)(HM *hm, const HM_KEY *k) {
    return _JOIN(HM, get_index)(hm, k) != (size_t)-1;
}

/**
 * @return Returns true if the value associated with k was successfully removed.
 */
bool JOIN(HM, remove)(HM *hm, const HM_KEY *k) {
    size_t cap = hm->cap;
    size_t mask = cap - 1;
    size_t i = _JOIN(HM, get_index)(hm, k);
    if (i == (size_t)-1) {
        return false;
    }
    size_t j = (i + 1) & mask;
    JOIN(HM_BUCKET, free)(&hm->data[i]);
    while (true) {
        hash h_at = hm->hashes[j];
        int dist_at = (j + cap - (h_at & mask)) & mask;
        if (h_at == 0 || dist_at == 0) {
            hm->hashes[i] = 0;
            return true;
        }
        hm->hashes[i] = hm->hashes[j];
        hm->data[i] = hm->data[j];
        i = j;
        j = (j + 1) & mask;
    }
}

HM JOIN(HM, copy)(const HM *hm) {
    hash *new_hashes = malloc(sizeof(hash) * hm->cap);
    memcpy(new_hashes, hm->hashes, sizeof(hash) * hm->cap);
    HM new_hm = {
        .len = hm->len,
        .cap = hm->cap,
        .threshold = hm->threshold,
        .hashes = new_hashes,
        .data = malloc(sizeof(HM_BUCKET) * hm->cap),
    };
#ifdef _HM_COPY_COMPLEX
    for (size_t i = 0; i < hm->cap; i++) {
        new_hm.data[i] = JOIN(HM_BUCKET, copy)(&hm->data[i]);
    }
#else
    memcpy(new_hm.data, hm->data, sizeof(HM_BUCKET) * hm->cap);
#endif
    return new_hm;
}

#ifndef HM_EXTEND
#undef _HM_FREE_COMPLEX
#undef _HM_COPY_COMPLEX
#undef HM_NAME
#undef HM
#undef HM_BUCKET
#undef HM_KEY
#undef HM_VALUE
#undef HM_KEY_FREE
#undef HM_KEY_COPY
#undef HM_VALUE_FREE
#undef HM_VALUE_COPY
#endif
