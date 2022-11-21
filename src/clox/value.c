#include "value.h"
#include "memory.h"

DYNAMIC_ARRAY_IMPL(lox_value)
DYNAMIC_ARRAY_IMPL(char)

typedef struct {
    lox_heap_object_type type;
    struct lox_heap_object* next;
} lox_heap_object;

typedef struct {
    lox_heap_object_type type;
    lox_heap_object* next;
    char_array chars;
} lox_heap_object_string;

void lox_value_print(lox_value value) {
    if (IS_NUMBER(value)) {
        printf("%g", value.as.number);
    } else if (IS_BOOL(value)) {
        printf(AS_BOOL(value) ? "true" : "false");
    } else if (IS_NIL(value)) {
        printf("nil");
    }
}
