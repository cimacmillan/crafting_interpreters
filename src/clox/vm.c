#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"

#define DEBUG_PRINT

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
    lox_value_array_init(&vm.stack);
}

void lox_vm_free() {
    lox_value_array_free(&vm.stack);
}

lox_vm_result lox_vm_run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.code[READ_BYTE()])
#define STACK_PUSH(value) (lox_value_array_add(&vm.stack, value))
#define STACK_POP() (lox_value_array_pop(&vm.stack))
#define MATH_BINARY(op) { \
    lox_value b = STACK_POP(); \
    lox_value a = STACK_POP(); \
    lox_value result = a op b; \
    STACK_PUSH(result); \
    break; \
}
    for (;;) {
        #ifdef DEBUG_PRINT
            printf("[stack] ");
            for (int i = vm.stack.size - 1; i >= 0; i--) {
                printf("[ ");
                lox_value_print(vm.stack.code[i]);
                printf(" ]");
            }
            printf("\n");
            disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->bytecode.code));
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
            case OP_NEGATE: {
                lox_value val = STACK_POP();
                STACK_PUSH(-val);  
                break; 
            }
            case OP_ADD: MATH_BINARY(+)
            case OP_SUB: MATH_BINARY(-)
            case OP_MUL: MATH_BINARY(*)
            case OP_DIV: MATH_BINARY(/)
        }
    }
    return LOX_VM_SUCCESS;
#undef READ_BYTE
#undef READ_CONSTANT
#undef STACK_PUSH
#undef STACK_POP
#undef MATH_BINARY
}


lox_vm_result interpret(char* source) {
    lox_chunk chunk;
    chunk_init(&chunk);
    bool result = compile(source, &chunk);
    if (!result) {
        chunk_free(&chunk);
        return LOX_VM_ERROR_COMPILATION;
    }

    disassemble_chunk(&chunk, "compilation result");

    lox_vm_init();

    vm.ip = chunk.bytecode.code;
    vm.chunk = &chunk;

    lox_vm_result vm_result = lox_vm_run();

    lox_vm_free();
    chunk_free(&chunk);

    return vm_result;
}
