#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

DYNAMIC_ARRAY_H(uint8_t)
DYNAMIC_ARRAY_H(int)

typedef enum {
    OP_RETURN, // Return from function
    OP_CONSTANT, // Load constant onto stack
    OP_NEGATE, // Unary negate number on top of stack
    OP_ADD, // Add top 2 stack values, push
    OP_SUB, // Sub top 2 stack values, push
    OP_MUL, // Mul top 2 stack values, push
    OP_DIV, // Div top 2 stack values, push
    OP_TRUE,
    OP_FALSE,
    OP_NIL,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
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
