#ifndef VEC_TYPE
#error "VEC_TYPE undefined"
// do this just to get better ide support
// the file will already fail to compile
#define VEC_TYPE int
#endif

#include "macro_util.h"
#include "prelude.h"
#include <stdlib.h>

#ifndef VEC
#define VEC JOIN(VEC_TYPE, vec)
#endif

typedef struct {
    size_t len;
    size_t cap;
    VEC_TYPE *data;
} VEC;

#define VEC_DEFINED

void JOIN(VEC, reserve)(VEC *vec, size_t i);

void JOIN(VEC, init)(VEC *vec);

VEC JOIN(VEC, new)(void);

VEC JOIN(VEC, new_with_cap)(size_t cap);

VEC JOIN(VEC, from_ptr_copied)(VEC_TYPE *p, size_t len);

void JOIN(VEC, push)(VEC *vec, VEC_TYPE x);

VEC_TYPE *JOIN(VEC, last)(const VEC *vec);

VEC_TYPE JOIN(VEC, pop)(VEC *vec);

VEC_TYPE JOIN(VEC, index)(const VEC *vec, int i);

void JOIN(VEC, free)(VEC *vec);

size_t JOIN(VEC, len)(const VEC *vec);

VEC JOIN(VEC, copy)(const VEC *vec);

VEC JOIN(VEC, take)(VEC *vec);

void JOIN(VEC, clear)(VEC *vec);

VEC_TYPE *JOIN(VEC, get_ptr)(VEC *vec, int i);

VEC_TYPE *JOIN(VEC, next_ptr)(VEC *vec);

#ifndef VEC_EXTEND
#undef VEC
#undef VEC_TYPE
#undef VEC_TYPE_FREE
#undef VEC_TYPE_COPY
#endif
