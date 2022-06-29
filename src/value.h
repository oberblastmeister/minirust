#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H

#include "prelude.h"

typedef struct {
    enum {
        VALUE_DOUBLE,
        VALUE_INT,
        VALUE_BOOL,
        VALUE_STRING,
        VALUE_NIL,
    } tag;
    union {
        double value_double;
        int value_int;
        bool value_bool;
    };
} value;

#define VEC_TYPE value
#include "vec_h.h"

void value_print(value value);

static inline value value_nil(void) {
    return (value){VALUE_NIL, {.value_int = 0}};
}

static inline bool value_is_nil(value value) { return value.tag == VALUE_NIL; }

static inline value value_double(double d) {
    return (value){VALUE_DOUBLE, {.value_double = d}};
}

static inline bool value_is_double(value value) {
    return value.tag == VALUE_DOUBLE;
}

static inline double value_as_double(value value) { return value.value_double; }

static inline value value_int(int i) {
    return (value){VALUE_INT, {.value_int = i}};
}

static inline bool value_is_int(value value) { return value.tag == VALUE_INT; }

static inline int value_as_int(value value) { return value.value_int; }

static inline value value_bool(bool b) {
    return (value){VALUE_BOOL, {.value_bool = b}};
}

static inline bool value_as_bool(value value) { return value.value_bool; }

static inline bool value_is_bool(value value) {
    return value.tag == VALUE_BOOL;
}

#endif
