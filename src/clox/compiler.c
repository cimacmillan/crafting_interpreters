#include "common.h"
#include "compiler.h"
#include "token.h"
#include "scanner.h"

void compile(char *source) {
    scanner_init(source);
    int line = -1;
    for (;;) {
        lox_token token = scanner_token();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        
        printf("%2d '%.*s'\n", token.type, token.length, token.start); 

        if (token.type == TOKEN_EOF) break;
    }
}
