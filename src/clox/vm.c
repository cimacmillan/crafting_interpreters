#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"

#define DEBUG_PRINT

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
}

void lox_vm_free() {

}

lox_vm_result lox_vm_run(lox_chunk *chunk) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (chunk->constants.code[READ_BYTE()])
    vm.ip = chunk->bytecode.code;
    for (;;) {
        #ifdef DEBUG_PRINT
            disassemble_instruction(chunk, (int)(vm.ip - chunk->bytecode.code));
        #endif
        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_RETURN: {
                return LOX_VM_SUCCESS;
            }
            case OP_CONSTANT: {
                lox_value val = READ_CONSTANT();
                lox_value_print(val);
                printf("\n");
                break;
            }
        }
    }
    return LOX_VM_SUCCESS;
#undef READ_BYTE
#undef READ_CONSTANT
}

