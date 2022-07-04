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

void string_free(string *string) { char_vec_free(string); }

void string_clear(string *string) { string->len = 1; }

string string_copy(string *string) { return char_vec_copy(string); }
