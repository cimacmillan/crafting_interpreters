#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define ARRAY_CAPACITY_GROW(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define ARRAY_GROW(array, type, old_capacity, new_capacity) \
    (type*)reallocate(array, sizeof(type) * old_capacity, sizeof(type) * new_capacity)

#define ARRAY_FREE(array, type, old_capacity) \
    (type*)reallocate(array, sizeof(type) * old_capacity, 0)

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

#endif
