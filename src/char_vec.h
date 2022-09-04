#pragma once

#include "hash.h"
#define VEC_TYPE char
#include "vec_h.h"

typedef char_vec string;

#define const_string_new(s) string_new_n((s), sizeof(s))

string string_new(char *s);

string string_new_n(char *s, size_t n);

void string_free(string *s);

void string_clear(string *s);

string string_copy(string *s);

bool string_eq(const string *s1, const string *s2);

void string_hash(hasher *h, const string *s);

void string_push(string *s, char c);
