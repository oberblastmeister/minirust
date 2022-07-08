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

bool string_eq(const string *s1, const string *s2) {
    return s1->len == s2->len && memcmp(s1->data, s2->data, s1->len - 1) == 0;
}

void string_hash(hasher *h, const string *s) {
    // this allows prefix freedom
    hash_size_t(h, &s->len);
    hash_bytes(h, s->data, s->len);
}
