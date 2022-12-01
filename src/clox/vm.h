#ifndef clox_vm_h
#define clox_vm_h

#include "common.h"
#include "chunk.h"
#include "hashmap.h"

struct lox_vm {
    lox_chunk *chunk;
    uint8_t *ip;
    lox_value_array stack;
    lox_heap_object *object_head;
    lox_hashmap intern_strings;
    lox_hashmap globals;
};
typedef struct lox_vm lox_vm;

typedef enum {
    LOX_VM_SUCCESS = 0,
    LOX_VM_ERROR_COMPILATION,
    LOX_VM_ERROR_RUNTIME
} lox_vm_result;

void lox_vm_init();
void lox_vm_free();
lox_vm_result interpret(char* source);
lox_vm_result lox_vm_run();

#endif

