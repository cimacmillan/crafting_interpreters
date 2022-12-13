#ifndef clox_vm_h
#define clox_vm_h

#include "common.h"
#include "chunk.h"
#include "hashmap.h"

#define LOX_VM_CALL_FRAME_MAX 256

struct lox_vm_call_frame {
    lox_chunk *chunk;
    uint8_t *ip;
    int stack_offset;
};
typedef struct lox_vm_call_frame lox_vm_call_frame;


struct lox_vm {
    lox_value_array stack;
    lox_heap_object *object_head;
    lox_hashmap intern_strings;
    lox_hashmap globals;
    lox_vm_call_frame call_frames[LOX_VM_CALL_FRAME_MAX];
    int call_frame;
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

