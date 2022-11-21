#ifndef clox_value_h
#define clox_value_h

#include "common.h"
#include "memory.h"

typedef enum {
    LOX_HEAP_OBJECT_TYPE_STRING
} lox_heap_object_type;

struct lox_heap_object;
struct lox_heap_object_string;

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

DYNAMIC_ARRAY_H(lox_value)
DYNAMIC_ARRAY_H(char)

// Is the lox value type of type
#define IS_NUMBER(val) (val.type == LOX_VALUE_TYPE_NUMBER)
#define IS_BOOL(val) (val.type == LOX_VALUE_TYPE_BOOL)
#define IS_NIL(val) (val.type == LOX_VALUE_TYPE_NIL)
#define IS_STRING(val) (val.type == LOX_VALUE_TYPE_HEAP_OBJ && val.as.obj->type == LOX_HEAP_OBJECT_TYPE_STRING)

// Extract the c value from the lox value type
#define AS_NUMBER(val) (val.as.number)
#define AS_BOOL(val) (val.as.boolean)
#define AS_STRING(val) (((lox_heap_object_string)val.as.obj)->chars)

#define TO_NUMBER(val) ((lox_value){ LOX_VALUE_TYPE_NUMBER, {.number=val} })
#define TO_BOOL(val) ((lox_value){ LOX_VALUE_TYPE_BOOL, {.boolean=val} })
#define TO_NIL ((lox_value){ LOX_VALUE_TYPE_NIL, {.number=0} })

void lox_value_print(lox_value value);

#endif
