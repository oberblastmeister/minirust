#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H

#include "prelude.h"

typedef struct {
    enum {
        VALUE_DOUBLE,
        VALUE_BOOL,
        VALUE_STRING,
    } tag;
    union {
        double value_double;
        bool bool_value;
    };
} value;

#define VEC_TYPE value
#include "vec_h.h"

void value_print(value value);

static inline value value_double(double d) {
    return (value){VALUE_DOUBLE, {.value_double = d}};
}

static inline bool value_is_double(value value) {
    return value.tag == VALUE_DOUBLE;
}

static inline double value_as_double(value value) {
    return value.value_double;
}

#endif
