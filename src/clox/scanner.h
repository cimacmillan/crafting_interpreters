#ifndef lox_scanner_h
#define lox_scanner_h

#include "token.h"

void scanner_init(const char* source);
lox_token scanner_token();

#endif
