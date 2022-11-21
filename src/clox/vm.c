#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"

#define DEBUG_PRINT

void runtime_error(const char* message) {
    fprintf(stderr, "Runtime Error: %s\n", message);
    exit(1);
}

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
    lox_value_array_init(&vm.stack);
}

void lox_vm_free() {
    lox_value_array_free(&vm.stack);
}

static lox_value peek(int distance) {
    return vm.stack.code[vm.stack.size - 1 - distance];
}

static bool is_falsey(lox_value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static bool is_equal(lox_value a, lox_value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case LOX_VALUE_TYPE_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case LOX_VALUE_TYPE_NIL:
            return true;
        case LOX_VALUE_TYPE_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            return false;
    }
    return false;
}

lox_vm_result lox_vm_run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.code[READ_BYTE()])
#define STACK_PUSH(value) (lox_value_array_add(&vm.stack, value))
#define STACK_POP() (lox_value_array_pop(&vm.stack))
#define MATH_BINARY(value_type, op) { \
    lox_value b = STACK_POP(); \
    lox_value a = STACK_POP(); \
    if (!IS_NUMBER(a) || !IS_NUMBER(b)) runtime_error("operand can only be used on number types"); \
    lox_value result = value_type(AS_NUMBER(a) op AS_NUMBER(b)); \
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
            case OP_TRUE: {
                STACK_PUSH(TO_BOOL(true));
                break;
            }
            case OP_FALSE: {
                STACK_PUSH(TO_BOOL(false));
                break;
            }
            case OP_NIL: {
                STACK_PUSH(TO_NIL);
                break;
            }
            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtime_error("can only negate a number");
                }
                lox_value val = STACK_POP();
                STACK_PUSH(TO_NUMBER(-AS_NUMBER(val)));  
                break; 
            }
            case OP_ADD: MATH_BINARY(TO_NUMBER, +)
            case OP_SUB: MATH_BINARY(TO_NUMBER, -)
            case OP_MUL: MATH_BINARY(TO_NUMBER, *)
            case OP_DIV: MATH_BINARY(TO_NUMBER, /)
            case OP_NOT: {
                lox_value val = STACK_POP();
                STACK_PUSH(TO_BOOL(is_falsey(val)));  
                break;
            }
            case OP_EQUAL: {
                lox_value a = STACK_POP();
                lox_value b = STACK_POP();
                STACK_PUSH(TO_BOOL(is_equal(a, b)));  
                break;
            }
            case OP_LESS: MATH_BINARY(TO_BOOL, <)
            case OP_GREATER: MATH_BINARY(TO_BOOL, >)
            default:
                runtime_error("vm does not recognise this opcode");
                break;
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
