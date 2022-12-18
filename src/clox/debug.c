#include "debug.h"

void disassemble_chunk(lox_chunk *chunk, char_array name) {
    printf("=== ");
    char_array_print(name);
    printf("===\n");
    for (int i = 0; i < chunk->bytecode.size;) {
        // Instructions can be of different size
        i = disassemble_instruction(chunk, i);
    }
    printf("=====================\n");
    for (int i = 0; i < chunk->constants.size; i++) {
        lox_value constant = chunk->constants.code[i];
        if (IS_FUNCTION(constant)) {
            lox_heap_object_function *func = AS_FUNCTION(constant);
            disassemble_chunk(&func->chunk, func->name);
        }
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

static int byte_arg_instruction(const char* name, lox_chunk *chunk, int offset) {
    uint8_t constant_address = chunk->bytecode.code[offset + 1];
    // -16 makes that much space so the numbers are then alligned
    printf("%-16s %04d", name, constant_address);
    printf("\n");
    return offset + 2;
}

static int short_arg_instruction(const char* name, lox_chunk *chunk, int offset) {
    uint16_t address = (chunk->bytecode.code[offset + 1] << 8) + chunk->bytecode.code[offset + 2] + offset + 3;
    // -16 makes that much space so the numbers are then alligned
    printf("%-16s -> %04d", name, address);
    printf("\n");
    return offset + 3;
}

static int loop_arg_instruction(const char* name, lox_chunk *chunk, int offset) {
    uint16_t jump = ((chunk->bytecode.code[offset + 1] << 8) + chunk->bytecode.code[offset + 2]);
    uint16_t address = offset - jump + 3;
    // -16 makes that much space so the numbers are then alligned
    printf("%-16s -> %04d", name, address);
    printf("\n");
    return offset + 3;
}

int disassemble_instruction(lox_chunk *chunk, int offset) {
#define SIMPLE_CASE(X)  \
    case X: \
        return simple_instruction(""#X, offset);

    (void)chunk;
    printf("%04d ", offset);
    int line_number = chunk->line_numbers.code[offset];
    // If the same line number made this bytecode, join it
    if (offset > 0 && line_number == chunk->line_numbers.code[offset - 1]) {
        printf("   | ");
    } else {
        printf("%04d ", line_number);
    }

    uint8_t code = chunk->bytecode.code[offset];
    switch (code) {
        SIMPLE_CASE(OP_RETURN)
        SIMPLE_CASE(OP_NEGATE)
        SIMPLE_CASE(OP_ADD)
        SIMPLE_CASE(OP_SUB)
        SIMPLE_CASE(OP_MUL)
        SIMPLE_CASE(OP_DIV)
        SIMPLE_CASE(OP_TRUE)
        SIMPLE_CASE(OP_FALSE)
        SIMPLE_CASE(OP_NIL)
        SIMPLE_CASE(OP_NOT)
        SIMPLE_CASE(OP_EQUAL)
        SIMPLE_CASE(OP_GREATER)
        SIMPLE_CASE(OP_LESS)
        SIMPLE_CASE(OP_PRINT)
        SIMPLE_CASE(OP_POP)
        case OP_CONSTANT:
            return constant_instruction("OP_CONSTANT", chunk, offset);
        case OP_DEFINE_GLOBAL:
            return constant_instruction("OP_DEFINE_GLOBAL", chunk, offset);
        case OP_GET_GLOBAL:
            return constant_instruction("OP_GET_GLOBAL", chunk, offset);
        case OP_SET_GLOBAL:
            return constant_instruction("OP_SET_GLOBAL", chunk, offset);
        case OP_GET_LOCAL:
            return byte_arg_instruction("OP_GET_LOCAL", chunk, offset);
        case OP_SET_LOCAL:
            return byte_arg_instruction("OP_SET_LOCAL", chunk, offset);
        case OP_JUMP:
            return short_arg_instruction("OP_JUMP", chunk, offset);
        case OP_JUMP_IF_FALSE:
            return short_arg_instruction("OP_JUMP_IF_FALSE", chunk, offset);
        case OP_LOOP:
            return loop_arg_instruction("OP_LOOP", chunk, offset);
        case OP_CALL:
            return byte_arg_instruction("OP_CALL", chunk, offset);
        case OP_CLOSURE: {
            offset++;
            uint8_t constant = chunk->bytecode.code[offset++];
            printf("%-16s %4d ", "OP_CLOSURE", constant);
            lox_value_print(chunk->constants.code[constant]);
            printf("\n");
            return offset;
        }
        default:
            printf("Unknown opcode %d\n", code);
            return offset + 1;
    }
    return 0;
#undef SIMPLE_CASE
}
