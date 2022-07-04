#ifndef _LOX_STRING_VEC_H

#define _LOX_STRING_VEC_H

#define VEC_TYPE char
#include "vec_h.h"

typedef char_vec string;

#define const_string_new(s) string_new_n((s), sizeof(s))

string string_new(char *s);

string string_new_n(char *s, size_t n);

void string_free(string *string_vec);

void string_clear(string *string);

string string_copy(string *string_vec);

#endif
