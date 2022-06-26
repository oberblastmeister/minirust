#include "value.h"
#include <stdio.h>

#define VEC_TYPE value
#include "vec.h"

void value_print(value value) { printf("%g", value); }
