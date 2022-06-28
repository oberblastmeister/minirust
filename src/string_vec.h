#ifndef _LOX_STRING_VEC_H

#define _LOX_STRING_VEC_H

#include "assert.h"
#include "uint8_t_vec.h"

typedef uint8_t_vec string_vec;

#define const_string_vec_new(s)                                                \
    _Generic((s), \
        char *: string_vec_new_n((s), sizeof(s)), \
        uint8_t *: string_vec_new_n((char *)(s), sizeof(s)))

string_vec string_vec_new(char *s);

string_vec string_vec_new_n(char *s, size_t n);

void string_vec_free(string_vec *string_vec);

string_vec string_vec_copy(string_vec *string_vec);

#endif
