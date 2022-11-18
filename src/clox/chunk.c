#include "chunk.h"
#include "memory.h"

void chunk_init(Chunk *chunk) {
    uint8_t_array_init(&(chunk->bytecode));    
}

void chunk_add(Chunk *chunk, uint8_t code) {
    uint8_t_array_add(&(chunk->bytecode), code);
}   

void chunk_free(Chunk *chunk) {
    uint8_t_array_free(&(chunk->bytecode));
}


