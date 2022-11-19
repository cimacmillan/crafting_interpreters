#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

#define DEBUG_PRINT

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
    lox_value_array_init(&vm.stack);
}

void lox_vm_free() {
    lox_value_array_free(&vm.stack);
}

lox_vm_result lox_vm_run(lox_chunk *chunk) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (chunk->constants.code[READ_BYTE()])
#define STACK_PUSH(value) (lox_value_array_add(&vm.stack, value))
    vm.ip = chunk->bytecode.code;
    for (;;) {
        #ifdef DEBUG_PRINT
            printf("[stack] ");
            for (int i = 0; i < vm.stack.size; i++) {
                printf("[ ");
                lox_value_print(vm.stack.code[i]);
                printf(" ]");
            }
            printf("\n");
            disassemble_instruction(chunk, (int)(vm.ip - chunk->bytecode.code));
        #endif
        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_RETURN: {
                return LOX_VM_SUCCESS;
            }
            case OP_CONSTANT: {
                lox_value val = READ_CONSTANT();
                STACK_PUSH(val);
                break;
            }
        }
    }
    return LOX_VM_SUCCESS;
#undef READ_BYTE
#undef READ_CONSTANT
}

