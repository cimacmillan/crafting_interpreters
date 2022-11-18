#include "debug.h"

void disassemble_chunk(Chunk *chunk, const char* name) {
    printf("=== %s ===\n", name);
    for (int i = 0; i < chunk->bytecode.size;) {
        // Instructions can be of different size
        i = disassemble_instruction(chunk, i);
    }
}

/**
 * @return int new offset
 */
static int simple_instruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

/**
 * @return int new offset
 */
int disassemble_instruction(Chunk *chunk, int offset) {
    (void)chunk;
    printf("%04d ", offset);
    uint8_t code = chunk->bytecode.code[offset];
    switch (code) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", code);
            return offset + 1;
    }
    return 0;
}
