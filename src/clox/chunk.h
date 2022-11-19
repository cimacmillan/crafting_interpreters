#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

DYNAMIC_ARRAY_H(uint8_t)
DYNAMIC_ARRAY_H(int)

typedef enum {
    OP_RETURN, // Return from function
    OP_CONSTANT, // Load constant
} lox_op_code;

typedef struct {
    uint8_t_array bytecode;
    lox_value_array constants;
    int_array line_numbers;
} lox_chunk;

void chunk_init(lox_chunk *chunk);
void chunk_free(lox_chunk *chunk);
void chunk_add_code(lox_chunk *chunk, uint8_t code, int line_number);
int chunk_add_constant(lox_chunk *chunk, lox_value constant);

#endif
