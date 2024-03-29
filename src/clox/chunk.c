#include "chunk.h"
#include "memory.h"

DYNAMIC_ARRAY_IMPL(uint8_t)
DYNAMIC_ARRAY_IMPL(int)

void chunk_init(lox_chunk *chunk) {
    uint8_t_array_init(&(chunk->bytecode));    
    lox_value_array_init(&(chunk->constants));
    int_array_init(&(chunk->line_numbers));
}

void chunk_add_code(lox_chunk *chunk, uint8_t code, int line_number) {
    uint8_t_array_add(&(chunk->bytecode), code);
    int_array_add(&(chunk->line_numbers), line_number);
}   

int chunk_add_constant(lox_chunk *chunk, lox_value code) {
    lox_value_array_add(&(chunk->constants), code);
    return chunk->constants.size - 1;
}   

void chunk_free(lox_chunk *chunk) {
    uint8_t_array_free(&(chunk->bytecode));
    lox_value_array_free(&(chunk->constants));
    int_array_free(&(chunk->line_numbers));
}
