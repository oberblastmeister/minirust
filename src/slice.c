#include "slice.h"
#include <stdlib.h>

void slice_free(slice *slice, void (*free_elem)(void *)) {
    for (size_t i = 0; i < slice->len; i++) {
        free_elem(slice->data + i);
    }
    free(slice->data);
}
