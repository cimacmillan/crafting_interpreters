#ifndef clox_value_h
#define clox_value_h

#include "common.h"
#include "memory.h"

typedef enum {
    LOX_VALUE_TYPE_NUMBER,
    LOX_VALUE_TYPE_BOOL,
    LOX_VALUE_TYPE_NIL,
    LOX_VALUE_TYPE_HEAP_OBJ
} lox_value_type;

typedef struct {
    lox_value_type type;
    union {
        double number;
        bool boolean;
        struct lox_heap_object *obj;
    } as;
} lox_value;

DYNAMIC_ARRAY_H(char)
DYNAMIC_ARRAY_H(uint8_t)
DYNAMIC_ARRAY_H(int)
DYNAMIC_ARRAY_H(lox_value)

typedef enum {
    OP_RETURN, // Return from function
    OP_CONSTANT, // Load constant onto stack
    OP_NEGATE, // Unary negate number on top of stack
    OP_ADD, // Add top 2 stack values, push
    OP_SUB, // Sub top 2 stack values, push
    OP_MUL, // Mul top 2 stack values, push
    OP_DIV, // Div top 2 stack values, push
    OP_TRUE,
    OP_FALSE,
    OP_NIL,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
    OP_LOOP,
    OP_CALL
} lox_op_code;

typedef struct {
    uint8_t_array bytecode;
    lox_value_array constants;
    int_array line_numbers;
} lox_chunk;

typedef enum {
    LOX_HEAP_OBJECT_TYPE_STRING,
    LOX_HEAP_OBJECT_TYPE_FUNCTION
} lox_heap_object_type;

struct lox_heap_object {
    lox_heap_object_type type;
    struct lox_heap_object* next;
};

typedef struct lox_heap_object lox_heap_object;

struct lox_heap_object_string {
    lox_heap_object_type type;
    struct lox_heap_object* next;
    char_array chars;
};

typedef struct lox_heap_object_string lox_heap_object_string;

struct lox_heap_object_function {
    lox_heap_object_type type;
    struct lox_heap_object* next;
    char_array name;
    lox_chunk chunk;
    int arity;
};

typedef struct lox_heap_object_function lox_heap_object_function;


// Is the lox value type of type
#define IS_NUMBER(val) (val.type == LOX_VALUE_TYPE_NUMBER)
#define IS_BOOL(val) (val.type == LOX_VALUE_TYPE_BOOL)
#define IS_NIL(val) (val.type == LOX_VALUE_TYPE_NIL)
#define IS_STRING(val) (val.type == LOX_VALUE_TYPE_HEAP_OBJ && val.as.obj->type == LOX_HEAP_OBJECT_TYPE_STRING)
#define IS_FUNCTION(val) (val.type == LOX_VALUE_TYPE_HEAP_OBJ && val.as.obj->type == LOX_HEAP_OBJECT_TYPE_FUNCTION)

// Extract the c value from the lox value type
#define AS_NUMBER(val) (val.as.number)
#define AS_BOOL(val) (val.as.boolean)
#define AS_OBJ(val) (val.as.obj)
#define AS_STRING(val) (((struct lox_heap_object_string *)val.as.obj))
#define AS_CSTRING(val) (((struct lox_heap_object_string *)val.as.obj)->chars.code)
#define AS_FUNCTION(val) ((struct lox_heap_object_function *)val.as.obj)

#define TO_NUMBER(val) ((lox_value){ LOX_VALUE_TYPE_NUMBER, {.number=val} })
#define TO_BOOL(val) ((lox_value){ LOX_VALUE_TYPE_BOOL, {.boolean=val} })
#define TO_NIL ((lox_value){ LOX_VALUE_TYPE_NIL, {.number=0} })
#define TO_OBJ(val) ((lox_value){ LOX_VALUE_TYPE_HEAP_OBJ, {.obj=val} })

struct lox_heap_object_string* new_lox_string(char* from, int length);
struct lox_heap_object_string* copy_lox_string(lox_heap_object_string* from);
void lox_value_print(lox_value value);
void free_obj(lox_heap_object *obj);
char_array char_array_from_const_char(const char *val);
void char_array_print(char_array array);
bool char_array_is_equal(char_array a, char_array b);

lox_heap_object_function* new_lox_function();


#endif
