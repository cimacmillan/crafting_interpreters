#include "debug.h"

void disassemble_chunk(lox_chunk *chunk, const char* name) {
    printf("=== %s ===\n", name);
    for (int i = 0; i < chunk->bytecode.size;) {
        // Instructions can be of different size
        i = disassemble_instruction(chunk, i);
    }
}

static int simple_instruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constant_instruction(const char* name, lox_chunk *chunk, int offset) {
    uint8_t constant_address = chunk->bytecode.code[offset + 1];
    // -16 makes that much space so the numbers are then alligned
    printf("%-16s %04d -> '", name, constant_address);
    lox_value_print(chunk->constants.code[constant_address]);
    printf("'\n");
    return offset + 2;
}

int disassemble_instruction(lox_chunk *chunk, int offset) {
    (void)chunk;
    printf("%04d ", offset);
    uint8_t code = chunk->bytecode.code[offset];
    switch (code) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constant_instruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown opcode %d\n", code);
            return offset + 1;
    }
    return 0;
}
