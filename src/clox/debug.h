#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassemble_chunk(lox_chunk *chunk, char_array name);
int disassemble_instruction(lox_chunk *chunk, int offset);

#endif
