#ifndef clox_value_h
#define clox_value_h

#include "common.h"
#include "memory.h"

typedef double lox_value;

DYNAMIC_ARRAY_H(lox_value)

void lox_value_print(lox_value value);

#endif
