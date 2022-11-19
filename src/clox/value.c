#include "value.h"
#include "memory.h"

DYNAMIC_ARRAY_IMPL(lox_value)

void lox_value_print(lox_value value) {
    printf("%g", value);
}

