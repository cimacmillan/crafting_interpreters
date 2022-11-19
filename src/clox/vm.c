#include "vm.h"
#include "chunk.h"
#include "debug.h"

#define DEBUG_PRINT

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
}

void lox_vm_free() {

}

lox_vm_result lox_vm_run(lox_chunk *chunk) {
#define READ_BYTE() (*vm.ip++)
    vm.ip = chunk->bytecode.code;
    for (;;) {
        #ifdef DEBUG_PRINT
            disassemble_instruction(chunk, (int)(vm.ip - chunk->bytecode.code));
        #endif
        uint8_t instruction = READ_BYTE();
        switch (instruction) {
            case OP_RETURN:
                return LOX_VM_SUCCESS;
        }
    }
#undef READ_BYTE

    return LOX_VM_SUCCESS;
}

