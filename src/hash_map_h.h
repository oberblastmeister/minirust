#include "hash.h"
#include "macro_util.h"
#include "prelude.h"

#ifndef HM_PREFIX
#define HM JOIN(JOIN(HM_VALUE, HM_KEY), map)
#else
#define HM JOIN(HM_PREFIX, map)
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
#endif

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

HM JOIN(HM, new)(void);

void JOIN(HM, free)(HM *hm);

bool JOIN(HM, insert)(HM *hm, HM_KEY k, HM_VALUE v);

HM_VALUE *JOIN(HM, get_ptr)(HM *hm, const HM_KEY *k);

bool JOIN(HM, contains)(HM *hm, const HM_KEY *k);

bool JOIN(HM, remove)(HM *hm, const HM_KEY *k);

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
#undef HM_KEY_HASH
#endif
