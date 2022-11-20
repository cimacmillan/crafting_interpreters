#include "common.h"
#include "compiler.h"
#include "token.h"
#include "scanner.h"
#include "chunk.h"

typedef struct {
    lox_token previous;
    lox_token current;
    bool had_error;
} lox_parser;

lox_parser parser;
lox_chunk *current_chunk;

static void error_at(lox_token* token, const char* message) {
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.had_error = true;
}

// static void error_at_previous(const char* message) {
//     error_at(&parser.previous, message);
// }

static void error_at_current(const char* message) {
    error_at(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanner_token();
        if (parser.current.type != TOKEN_ERROR) break;
        error_at_current(parser.current.start);
    }
}

static void emit_byte(uint8_t code) {
    chunk_add_code(current_chunk, code, parser.previous.line);
}

static void emit_bytes(uint8_t code, uint8_t code2) {
    emit_byte(code);
    emit_byte(code2);
}

static void emit_constant(lox_value value) {
    int val = chunk_add_constant(current_chunk, value);
    emit_bytes(OP_CONSTANT, val);
}

static void number() {
    lox_value value = strtod(parser.current.start, NULL);
    emit_constant(value);
}

static void unary() {
    lox_token_type previous = parser.previous.type;
    number();

    if (previous == TOKEN_MINUS) {
        emit_byte(OP_NEGATE);
    }
    return;
}

static void expression() {
    advance();
    unary();
}

bool compile(char *source, lox_chunk *chunk) {
    current_chunk = chunk;
    parser.had_error = false;
    scanner_init(source);
    advance();
    expression();
    chunk_add_code(chunk, OP_RETURN, 0);
    return !parser.had_error;
}
