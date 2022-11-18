#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"

typedef enum {
    OP_RETURN, // Return from function
} OpCode;

typedef struct {
    uint8_t_array bytecode;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_add(Chunk *chunk, uint8_t code);
void chunk_free(Chunk *chunk);

#endif
