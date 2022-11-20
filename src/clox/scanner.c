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

static char peek() {
    return *scanner.current;
}

static char peek_next() {
    if (is_at_end()) return '\0';
    return scanner.current[1];
}

static void skip_whitespace() {
    while (true) {
        char val = peek();
        switch (val) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peek_next() == '/') {
                    while (peek() != '\n' && !is_at_end()) advance();
                } else {
                    return;
                }
                break;
            default:
                return; 
        }
    }
}

static lox_token string() {
    while(peek() != '"' && !is_at_end()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (is_at_end()) return error_token("unterminated string");

    advance();
    return make_token(TOKEN_STRING);
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static lox_token number() {
    while (is_digit(peek())) advance();

    if (peek() == '.' && is_digit(peek_next())) {
        advance();
        while (is_digit(peek())) advance();
    }

    return make_token(TOKEN_NUMBER);
}

static bool is_alpha(char c) {
    return (
        (c >= 'a' && c <='z') || 
        (c >= 'A' && c <= 'Z') ||
        c == '_'
    );
}

static bool check_keyword(const char* keyword) {
    int length = strlen(keyword);
    int scanned_length = (int)(scanner.current - scanner.start);
    if (length != scanned_length) {
        return false;
    }
    bool is_match = memcmp(scanner.start, keyword, length) == 0;
    return is_match;
}

static lox_token_type identifier_type() {
#define MATCH(identifer, type) if (check_keyword(identifer)) return type;
    MATCH("and", TOKEN_AND);
    MATCH("class", TOKEN_CLASS);
    MATCH("else", TOKEN_ELSE);
    MATCH("if", TOKEN_IF);
    MATCH("nil", TOKEN_NIL);
    MATCH("or", TOKEN_OR);
    MATCH("print", TOKEN_PRINT);
    MATCH("return", TOKEN_RETURN);
    MATCH("super", TOKEN_SUPER);
    MATCH("var", TOKEN_VAR);
    MATCH("while", TOKEN_WHILE);
    MATCH("false", TOKEN_FALSE);
    MATCH("for", TOKEN_FOR);
    MATCH("fun", TOKEN_FUN);
    MATCH("this", TOKEN_THIS);
    MATCH("true", TOKEN_TRUE);
    return TOKEN_IDENTIFIER;
#undef MATCH
}

static lox_token identifier() {
    while (is_alpha(peek()) || is_digit(peek())) advance();
    return make_token(identifier_type());
}

lox_token scanner_token() {
# define SINGLE_CHAR(ch, token_type) case(ch): return make_token(token_type);
# define DOUBLE_CHAR(ch_a, ch_b, token_type_a, token_type_b) case(ch_a): return (match(ch_b) ? make_token(token_type_b) : make_token(token_type_a));
    skip_whitespace();
    scanner.start = scanner.current;

    if (is_at_end()) {
        return make_token(TOKEN_EOF);
    }

    char character = advance();
    if (is_digit(character)) return number();
    if (is_alpha(character)) return identifier();

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
        case '"':
            return string();
    }

    return error_token("Unexpected character.");
#undef SINGLE_CHAR
#undef DOUBLE_CHAR
}


