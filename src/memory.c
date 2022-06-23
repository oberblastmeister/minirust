#include <stdlib.h>

void *reallocate(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        exit(1);
    }
    return new_ptr;
}
