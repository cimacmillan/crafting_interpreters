#include "scanner.h"
#include "token.h"
#include "common.h"

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

static lox_token make_token(lox_token_type type) {
    lox_token token;
    token.type = type;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    token.start = scanner.start;
    return token;
}

static lox_token error_token(const char *error) {
    lox_token token;
    token.type = TOKEN_ERROR;
    token.length = strlen(error);
    token.line = scanner.line;
    token.start = error;
    return token;
} 

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static bool is_at_end() {
    return *scanner.current == '\0';
}

static bool match(char a) {
    if (is_at_end()) return false;
    if (*scanner.current == a) {
        advance();
        return true;
    }
    return false;
}

lox_token scanner_token() {
# define SINGLE_CHAR(ch, token_type) case(ch): return make_token(token_type);
# define DOUBLE_CHAR(ch_a, ch_b, token_type_a, token_type_b) case(ch_a): return (match(ch_b) ? make_token(token_type_b) : make_token(token_type_a));

    scanner.start = scanner.current;

    if (is_at_end()) {
        return make_token(TOKEN_EOF);
    }

    char character = advance();

    switch (character) {
        SINGLE_CHAR('(', TOKEN_LEFT_PAREN)
        SINGLE_CHAR(')', TOKEN_RIGHT_PAREN)
        SINGLE_CHAR('{', TOKEN_LEFT_BRACE)
        SINGLE_CHAR('}', TOKEN_RIGHT_BRACE)
        SINGLE_CHAR(';', TOKEN_SEMICOLON)
        SINGLE_CHAR(',', TOKEN_COMMA)
        SINGLE_CHAR('.', TOKEN_DOT)
        SINGLE_CHAR('-', TOKEN_MINUS)
        SINGLE_CHAR('+', TOKEN_PLUS)
        SINGLE_CHAR('/', TOKEN_SLASH)
        SINGLE_CHAR('*', TOKEN_STAR)
        DOUBLE_CHAR('!', '=', TOKEN_BANG, TOKEN_BANG_EQUAL)
        DOUBLE_CHAR('=', '=', TOKEN_EQUAL, TOKEN_EQUAL_EQUAL)
        DOUBLE_CHAR('<', '=', TOKEN_LESS, TOKEN_LESS_EQUAL)
        DOUBLE_CHAR('>', '=', TOKEN_GREATER, TOKEN_GREATER_EQUAL)
    }

    return error_token("Unexpected character.");
#undef SINGLE_CHAR
}


