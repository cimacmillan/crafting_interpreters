#include "chunk.h"
#include "memory.h"

void chunk_init(Chunk *chunk) {
    chunk->size = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void chunk_add(Chunk *chunk, uint8_t code) {
    if (chunk->capacity <= chunk->size + 1) {
        int old_capacity = chunk->capacity;
        chunk->capacity = ARRAY_CAPACITY_GROW(chunk->capacity);
        chunk->code = ARRAY_GROW(chunk->code, uint8_t, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->size] = code;
    chunk->size++;
}   

void chunk_free(Chunk *chunk) {
    int old_capacity = chunk->capacity;
    ARRAY_FREE(chunk->code, uint8_t, old_capacity);
    chunk->capacity = 0;
    chunk->size = 0;
}


