#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

void chunk_init(lox_chunk *chunk);
void chunk_free(lox_chunk *chunk);
void chunk_add_code(lox_chunk *chunk, uint8_t code, int line_number);
int chunk_add_constant(lox_chunk *chunk, lox_value constant);

#endif
