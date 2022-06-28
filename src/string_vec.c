#include "string_vec.h"
#include "uint8_t_vec.h"
#include <string.h>

string_vec string_vec_new(char *s) {
    // strlen returns the length not including the null character
    unsigned long len = strlen(s);
    uint8_t_vec res = string_vec_new_n(s, (size_t)len + 1);
    return res;
}

uint8_t_vec string_vec_new_n(char *s, size_t n) {
    return uint8_t_vec_from_ptr_copied((uint8_t *)s, n);
}

void string_vec_free(string_vec *string_vec) {
    uint8_t_vec_free(string_vec);
}

string_vec string_vec_copy(string_vec *string_vec) {
    return uint8_t_vec_copy(string_vec);
}
