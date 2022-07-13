#include "object.h"
#include "value.h"
#include <stdio.h>

static inline void fun_print(obj_fun *fun) { printf("<fn %s>", fun->name); }

void obj_print(value obj) {
    switch (value_as_obj(obj)->type) {
    case OBJ_STRING: {
        printf("\"%s\"", value_as_cstring(obj));
        break;
    }
    case OBJ_FUN: {
        fun_print(value_as_fun(obj));
        break;
    }
    default: {
        printf("<unknown object>");
        break;
    }
    }
}

obj_string *obj_string_new(char *s, size_t len, obj **objects) {
    obj_string *string =
        (obj_string *)alloc_obj(sizeof(obj_string), OBJ_STRING, objects);
    string->len = len;
    string->s = s;
    return string;
}

obj_fun *obj_fun_new(obj **objects) {
    obj_fun *fun = (obj_fun *)alloc_obj(sizeof(obj_fun), OBJ_FUN, objects);
    fun->arity = 0;
    fun->name = NULL;
    fun->chunk = chunk_new();
    return fun;
}

void obj_free(obj *obj) {
    switch (obj->type) {
    case OBJ_STRING: {
        obj_string *string = (obj_string *)obj;
        free(string->s);
        free(obj);
        break;
    }
    case OBJ_FUN: {
        obj_fun *fun = (obj_fun *)obj;
        chunk_free(&fun->chunk);
        free(fun->name);
        break;
    }
    }
}
