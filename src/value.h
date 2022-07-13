#ifndef _LOX_VALUE_H
#define _LOX_VALUE_H

#include "char_vec.h"
#include "prelude.h"

typedef struct obj obj;

typedef struct obj_string obj_string;

typedef struct {
    enum {
        VALUE_NIL,
        VALUE_DOUBLE,
        VALUE_INT,
        VALUE_BOOL,
        VALUE_OBJ,
    } type;
    union {
        int value_nil;
        double value_double;
        int value_int;
        bool value_bool;
        obj *value_obj;
    };
} value;

#define VEC_TYPE value
#include "vec_h.h"

void obj_print(value obj);

void value_print(value value);

static inline value value_nil(void) {
    return (value){VALUE_NIL, {.value_int = 0}};
}

static inline bool value_is_nil(value value) { return value.type == VALUE_NIL; }

static inline value value_double(double d) {
    return (value){VALUE_DOUBLE, {.value_double = d}};
}

static inline bool value_is_double(value value) {
    return value.type == VALUE_DOUBLE;
}

static inline double value_as_double(value value) { return value.value_double; }

static inline value value_int(int i) {
    return (value){VALUE_INT, {.value_int = i}};
}

static inline bool value_is_int(value value) { return value.type == VALUE_INT; }

static inline int value_as_int(value value) { return value.value_int; }

static inline value value_bool(bool b) {
    return (value){VALUE_BOOL, {.value_bool = b}};
}

static inline bool value_as_bool(value value) { return value.value_bool; }

static inline bool value_is_bool(value value) {
    return value.type == VALUE_BOOL;
}

static inline bool value_is_obj(value value) { return value.type == VALUE_OBJ; }

static inline obj *value_as_obj(value value) { return value.value_obj; }

static inline value value_obj(obj *obj) {
    return (value){VALUE_OBJ, {.value_obj = obj}};
}

static inline bool value_equal(value v1, value v2) {
#define BOTH_IS(T) (value_is_##T(v1) && value_is_##T(v2))
#define EQ_SIMPLE(T)                                                           \
    (value_is_##T(v1) && value_is_##T(v2) &&                                   \
     (value_as_##T(v1) == value_as_##T(v2)))
#define X EQ_SIMPLE
    return BOTH_IS(nil) || EQ_SIMPLE(double) || EQ_SIMPLE(int) ||
           EQ_SIMPLE(bool) || EQ_SIMPLE(obj);
#undef BOTH_IS
#undef EQ_SIMPLE
#undef X
}

#endif
