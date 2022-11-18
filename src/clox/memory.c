#include "memory.h"

DYNAMIC_ARRAY_IMPL(uint8_t)

void* reallocate(void* pointer, size_t before, size_t after) {
    (void)before;
    if (after == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, after);

    if (result == NULL) {
        fprintf(stderr, "failed to allocate memory for dynamic array");
    }

    return result;
}
