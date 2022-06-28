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

#endif
