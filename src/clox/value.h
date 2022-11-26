#ifndef clox_value_h
#define clox_value_h

#include "common.h"
#include "memory.h"

DYNAMIC_ARRAY_H(char)

typedef enum {
    LOX_HEAP_OBJECT_TYPE_STRING
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

// Is the lox value type of type
#define IS_NUMBER(val) (val.type == LOX_VALUE_TYPE_NUMBER)
#define IS_BOOL(val) (val.type == LOX_VALUE_TYPE_BOOL)
#define IS_NIL(val) (val.type == LOX_VALUE_TYPE_NIL)
#define IS_STRING(val) (val.type == LOX_VALUE_TYPE_HEAP_OBJ && val.as.obj->type == LOX_HEAP_OBJECT_TYPE_STRING)

// Extract the c value from the lox value type
#define AS_NUMBER(val) (val.as.number)
#define AS_BOOL(val) (val.as.boolean)
#define AS_OBJ(val) (val.as.obj)
#define AS_STRING(val) (((struct lox_heap_object_string *)val.as.obj))
#define AS_CSTRING(val) (((struct lox_heap_object_string *)val.as.obj)->chars.code)


#define TO_NUMBER(val) ((lox_value){ LOX_VALUE_TYPE_NUMBER, {.number=val} })
#define TO_BOOL(val) ((lox_value){ LOX_VALUE_TYPE_BOOL, {.boolean=val} })
#define TO_NIL ((lox_value){ LOX_VALUE_TYPE_NIL, {.number=0} })
#define TO_OBJ(val) ((lox_value){ LOX_VALUE_TYPE_HEAP_OBJ, {.obj=val} })

struct lox_heap_object_string* new_lox_string(char* from, int length);
struct lox_heap_object_string* copy_lox_string(lox_heap_object_string* from);
void lox_value_print(lox_value value);
void free_obj(lox_heap_object *obj);

#endif
