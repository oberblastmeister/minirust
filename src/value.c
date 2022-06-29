#include "value.h"
#include <stdio.h>

#define VEC_TYPE value
#include "vec.h"

void value_print(value value) {
    if (value_is_double(value)) {
        printf("%lf", value_as_double(value));
    } else if (value_is_int(value)) {
        printf("%d", value_as_int(value));
    } else {
        printf("<unknown>");
    }
}
