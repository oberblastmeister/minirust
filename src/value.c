#include "value.h"
#include "io_ext.h"
#include <stdio.h>

#define VEC_TYPE value
#include "vec.h"

static void obj_print(value obj) {
    switch (value_as_obj(obj)->type) {
    case OBJ_STRING: {
        printf("\"%s\"", value_as_cstring(obj));
        break;
    }
    default: {
        printf("<unknown object>");
        break;
    }
    }
}

void value_print(value value) {
    if (value_is_nil(value)) {
        printf("nil");
    } else if (value_is_double(value)) {
        printf("%lf", value_as_double(value));
    } else if (value_is_int(value)) {
        printf("%d", value_as_int(value));
    } else if (value_is_bool(value)) {
        puts(value_as_bool(value) ? "true" : "false");
    } else if (value_is_obj(value)) {
        obj_print(value);
    } else {
        printf("<unknown>");
    }
}

obj_string *alloc_string(char *s, size_t len, obj **objects) {
    obj_string *string =
        (obj_string *)alloc_obj(sizeof(obj_string), OBJ_STRING, objects);
    string->len = len;
    string->s = s;
    return string;
}

void obj_free(obj *obj) {
    switch (obj->type) {
    case OBJ_STRING: {
        obj_string *string = (obj_string *)obj;
        free(string->s);
        free(obj);
        break;
    }
    }
}
