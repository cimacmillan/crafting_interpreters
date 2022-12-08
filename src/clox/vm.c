#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"

#define DEBUG_PRINT

lox_vm vm;

void runtime_error(const char* message) {
    fprintf(stderr, "Runtime Error: %s\n", message);
    exit(1);
}

void lox_vm_init() {
    vm.chunk = NULL;
    vm.ip = NULL;
    vm.object_head = NULL;
    lox_value_array_init(&vm.stack);
    lox_hashmap_init(&vm.intern_strings);
    lox_hashmap_init(&vm.globals);
}

void lox_vm_free() {
    lox_value_array_free(&vm.stack);
    lox_heap_object *obj = vm.object_head;
    while(obj) {
        lox_heap_object *next = obj->next;
        free_obj(obj);
        obj = next;
    }
}

static lox_value peek(int distance) {
    return vm.stack.code[vm.stack.size - 1 - distance];
}

static void on_lox_object_allocation(lox_heap_object *obj) {
    obj->next = vm.object_head;
    vm.object_head = obj;
}

static bool is_falsey(lox_value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static bool is_heap_obj_equal(lox_heap_object *a, lox_heap_object *b) {
    if (a->type != b->type) return false;
    switch (a->type) {
        case LOX_HEAP_OBJECT_TYPE_STRING: {
            lox_heap_object_string *a_str = (lox_heap_object_string*)a;
            lox_heap_object_string *b_str = (lox_heap_object_string*)b;
            if (a_str->chars.size != b_str->chars.size) return false;
            for (int i = 0; i < a_str->chars.size; i++) {
                if (a_str->chars.code[i] != b_str->chars.code[i]) return false;
            }
            return true;
        }
    }
    return false;
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
        case LOX_VALUE_TYPE_HEAP_OBJ: 
            return is_heap_obj_equal(AS_OBJ(a), AS_OBJ(b));
        default:
            return false;
    }
    return false;
}

static lox_value get_merged_string(lox_heap_object_string *a, lox_heap_object_string *b) {
    char_array concat;
    char_array_merge(&concat, &a->chars, &b->chars);
    lox_value *intern_str = lox_hashmap_get(&vm.intern_strings, concat);
    if (intern_str != NULL) {
        return *intern_str;
    }
    lox_heap_object_string *str = new_lox_string(concat.code, concat.size);
    on_lox_object_allocation((lox_heap_object*)str);
    lox_value result = TO_OBJ((lox_heap_object*)str);
    lox_hashmap_insert(&vm.intern_strings, concat, result);
    return result;
}

lox_vm_result lox_vm_run() {
#define READ_BYTE() (*vm.ip++)
#define READ_SHORT() \
    (vm.ip += 2, (uint16_t)((vm.ip[-2] << 8) | vm.ip[-1]))
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
            case OP_ADD: { 
                lox_value b = STACK_POP(); 
                lox_value a = STACK_POP(); 
                if (IS_STRING(a) && IS_STRING(b)) {
                    lox_value result = get_merged_string(AS_STRING(a), AS_STRING(b));
                    STACK_PUSH(result);
                } else if (IS_NUMBER(a) && IS_NUMBER(b)){
                    lox_value result = TO_NUMBER(AS_NUMBER(a) + AS_NUMBER(b)); 
                    STACK_PUSH(result);
                } else {
                    runtime_error("+ operand can only be used on number or string types");
                }
                break;
            }
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
            case OP_PRINT: {
                lox_value a = STACK_POP();
                lox_value_print(a);
                printf("\n");
                break;
            }
            case OP_POP: {
                STACK_POP();
                break;
            }
            case OP_DEFINE_GLOBAL: {
                lox_value var_name = READ_CONSTANT();
                lox_value value = STACK_POP();
                lox_hashmap_insert(&vm.globals, AS_STRING(var_name)->chars, value);
                lox_hashmap_print(&vm.globals);
                break;
            }
            case OP_GET_GLOBAL: {
                lox_value var_name = READ_CONSTANT();
                lox_value *value = lox_hashmap_get(&vm.globals, AS_STRING(var_name)->chars);
                if (value == NULL) {
                    lox_value_print(var_name);
                    runtime_error("no variable with this name");
                }
                STACK_PUSH(*value);
                break;
            }
            case OP_SET_GLOBAL: {
                lox_value var_name = READ_CONSTANT();
                lox_value value = STACK_POP();
                if (!lox_hashmap_contains(&vm.globals, AS_STRING(var_name)->chars)) {
                    lox_value_print(var_name);
                    runtime_error("undefined variable. define it first with var");
                }
                lox_hashmap_insert(&vm.globals, AS_STRING(var_name)->chars, value);
                lox_hashmap_print(&vm.globals);
                break;
            }
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                STACK_PUSH(vm.stack.code[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                vm.stack.code[slot] = peek(0);
                break;
            }
            case OP_JUMP_IF_FALSE: {
                lox_value value = peek(0);
                uint16_t jump = READ_SHORT();
                if (is_falsey(value)) {
                    vm.ip += jump;
                }
                break;
            }
            case OP_JUMP: {
                uint16_t jump = READ_SHORT();
                vm.ip += jump;
                break;
            }
            default:
                printf("%d - \n", instruction);
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
