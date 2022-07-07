#include "char_vec.h"
#include <string.h>

#define VEC_TYPE char
#include "vec.h"

string string_new(char *s) {
    // strlen returns the length not including the null character
    unsigned long len = strlen(s);
    return string_new_n(s, (size_t)len + 1);
}

string string_new_n(char *s, size_t n) {
    return char_vec_from_ptr_copied(s, n);
}

void string_free(string *s) { char_vec_free(s); }

void string_clear(string *s) { s->len = 1; }

string string_copy(string *s) { return char_vec_copy(s); }

string string_eq(string *s1, string *s2);
