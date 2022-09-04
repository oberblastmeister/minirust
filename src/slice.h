#pragma once

#include <stddef.h>

typedef struct {
    size_t len;
    void *data;
} slice;

void slice_free(slice *slice, void (*free_elem)(void *));
