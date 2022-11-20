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

static void expression() {

}

bool compile(char *source, lox_chunk *chunk) {
    parser.had_error = false;
    scanner_init(source);
    advance();
    expression();
    chunk_add_code(chunk, OP_RETURN, 0);
    return !parser.had_error;
}
