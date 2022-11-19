#ifndef clox_vm_h
#define clox_vm_h

#include "common.h"
#include "chunk.h"

typedef struct {
    lox_chunk *chunk;
    uint8_t *ip;
    lox_value_array stack;
} lox_vm;

typedef enum {
    LOX_VM_SUCCESS = 0,
    LOX_VM_ERROR_COMPILATION,
    LOX_VM_ERROR_RUNTIME
} lox_vm_result;

lox_vm vm;

void lox_vm_init();
void lox_vm_free();
lox_vm_result interpret(char* source);
lox_vm_result lox_vm_run(lox_chunk *chunk);

#endif

