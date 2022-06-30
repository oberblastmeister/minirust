#ifndef _LOX_HASH_H
#define _LOX_HASH_H

#include "prelude.h"

#if __SIZEOF_SIZE_T__ == 8
#define FX_CONST 0x517cc1b727220a95
#elif __SIZEOF_SIZE_T__ == 4
#define FX_CONST 0x9e3779b9
#else
#error "size_t must be either 8 or 4 bytes for hash.h to work"
#endif

typedef size_t hash;

static inline add_size_t(hash h, size_t i) {

}

#endif
