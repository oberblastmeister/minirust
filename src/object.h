#ifndef _LOX_OBJECT_H
#define _LOX_OBJECT_H

#include "chunk.h"
#include "prelude.h"
#include "value.h"

typedef enum { OBJ_STRING, OBJ_FUN } obj_type;

typedef struct obj {
    obj_type type;
    struct obj *next;
} obj;

typedef struct obj_string {
    obj obj;
    size_t len;
    char *s;
} obj_string;

typedef struct obj_fun {
    obj obj;
    int arity;
    chunk chunk;
    char *name;
} obj_fun;

static inline bool is_obj_type(value value, obj_type type) {
    return value_is_obj(value) && value_as_obj(value)->type == type;
}

static inline obj_string *value_as_string(value value) {
    return (obj_string *)value_as_obj(value);
}

static inline char *value_as_cstring(value value) {
    return value_as_string(value)->s;
}

static inline obj *alloc_obj(size_t size, obj_type type, obj **objects) {
    obj *object = (obj *)malloc(size);
    object->type = type;

    object->next = *objects;
    *objects = object;

    return object;
}

obj_string *obj_string_new(char *s, size_t len, obj **objects);

obj_fun *obj_fun_new(obj **objects);

void obj_free(obj *obj);

obj_fun *obj_fun_new(obj **objects);

static inline obj_fun *value_as_fun(value value) {
    return (obj_fun *)value_as_obj(value);
}

static inline bool value_is_fun(value value) {
    return is_obj_type(value, OBJ_FUN);
}

#endif
