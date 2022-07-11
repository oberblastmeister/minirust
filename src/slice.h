#ifndef _LOX_SLICE_H
#define _LOX_SLICE_H

#include <stddef.h>

typedef struct {
    size_t len;
    void *data;
} slice;

void slice_free(slice *slice, void (*free_elem)(void *));
#endif
