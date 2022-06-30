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

#ifndef HM_EXTEND
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
