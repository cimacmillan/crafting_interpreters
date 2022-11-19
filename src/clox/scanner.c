#include "scanner.h"
#include "token.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
} lox_scanner;

lox_scanner scanner;

void scanner_init(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}   

lox_token scanner_token() {
    lox_token result = { 0 };
    return result;
}


