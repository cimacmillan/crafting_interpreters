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

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} lox_precedence;

typedef void (*parse_fn)(bool can_assign);

typedef struct {
  parse_fn prefix;
  parse_fn infix;
  lox_precedence precedence;
} lox_parse_rule;

lox_parser parser;
lox_chunk *current_chunk;

static lox_parse_rule* get_rule(lox_token_type type);
static void parse_precedence(lox_precedence precedence);
static void expression();

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
  exit(1);
}

static void error_at_previous(const char* message) {
    error_at(&parser.previous, message);
}

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

static bool check(lox_token_type type) {
    return parser.current.type == type;
}

static bool match(lox_token_type type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

static void consume(lox_token_type type, const char* error) {
    advance();
    if (parser.previous.type != type) {
        error_at_previous(error);
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

static void string(bool can_assign) {
    (void)can_assign;
    lox_token token = parser.previous;
    lox_value value = TO_OBJ((lox_heap_object*)new_lox_string((char*)token.start + 1, token.length - 2));
    emit_constant(value);
}

static void literal(bool can_assign) {
    (void)can_assign;
    switch (parser.previous.type) {
        case TOKEN_NUMBER: {
            lox_value value = TO_NUMBER(strtod(parser.previous.start, NULL));
            emit_constant(value);
            break;
        }
        case TOKEN_TRUE: {
            emit_byte(OP_TRUE);
            break;
        }
        case TOKEN_FALSE: {
            emit_byte(OP_FALSE);
            break;
        }
        case TOKEN_NIL: {
            emit_byte(OP_NIL);
            break;
        }
        default:
            error_at_previous("Unknown literal type");
            break;
    }
}

static void unary(bool can_assign) {
    (void)can_assign;
    lox_token_type previous = parser.previous.type;
    parse_precedence(PREC_UNARY);
    if (previous == TOKEN_MINUS) {
        emit_byte(OP_NEGATE);
    } else if (previous == TOKEN_BANG) {
        emit_byte(OP_NOT);
    }
    return;
}

static void grouping(bool can_assign) {
    (void)can_assign;
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void binary(bool can_assign) {
    (void)can_assign;
    lox_token_type operator = parser.previous.type;
    lox_parse_rule* rule = get_rule(operator);
    // Only if next rule is factor do we parse more of the expression. 
    // Otherwise we can just add whatever is on the stack to the next full parse.
    // Eg 1 + 2 * 4 OR 1 + 2 + 3
    // = 1 + (2 * 4) | (1 + 2) + 3
    parse_precedence((lox_precedence)(rule->precedence + 1));
    if (operator == TOKEN_PLUS) {
        emit_byte(OP_ADD);
    } else if(operator == TOKEN_MINUS) {
        emit_byte(OP_SUB);
    } else if(operator == TOKEN_STAR) {
        emit_byte(OP_MUL);
    } else if(operator == TOKEN_SLASH) {
        emit_byte(OP_DIV);
    } else if (operator == TOKEN_EQUAL_EQUAL) {
        emit_byte(OP_EQUAL);
    } else if (operator == TOKEN_BANG_EQUAL) {
        emit_bytes(OP_EQUAL, OP_NOT);
    } else if (operator == TOKEN_GREATER) {
        emit_byte(OP_GREATER);
    } else if (operator == TOKEN_GREATER_EQUAL) {
        emit_bytes(OP_LESS, OP_NOT);
    } else if (operator == TOKEN_LESS) {
        emit_byte(OP_LESS);
    } else if (operator == TOKEN_LESS_EQUAL) {
        emit_bytes(OP_GREATER, OP_NOT);
    }
}

static int identifier_constant() {
    lox_heap_object_string *str = new_lox_string((char*)parser.previous.start, parser.previous.length);
    return chunk_add_constant(current_chunk, TO_OBJ((lox_heap_object*)str));
}

static void variable(bool can_assign) {
    int value = identifier_constant();

    if (!can_assign && match(TOKEN_EQUAL)) {
        error_at_previous("Cannot assign to anything other than variable");
    }

    if (match(TOKEN_EQUAL)) {
        expression();
        emit_bytes(OP_SET_GLOBAL, value);
    } else {
        emit_bytes(OP_GET_GLOBAL, value);
    }
}

lox_parse_rule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {unary,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary,   PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     binary,   PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_IDENTIFIER]    = {variable,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {string,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {literal,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {literal,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static lox_parse_rule* get_rule(lox_token_type type) {
  return &rules[type];
}

static void parse_precedence(lox_precedence precedence) {
    (void)precedence;
    // Consume the first token
    // | 1 + 2 => 1 | + 2
    advance();
    lox_parse_rule *rule = get_rule(parser.previous.type);
    if (rule == NULL) {
        error_at_previous("No parse rule for token");
    }
    if (rule->prefix == NULL) {
        error_at_previous("No prefix rule for token");
    }
    bool can_assign = precedence <= PREC_ASSIGNMENT;
    // (1) is parsed as number()
    rule->prefix(can_assign);
    while(precedence <= get_rule(parser.current.type)->precedence) {
        advance();
        rule = get_rule(parser.previous.type);
        if (rule == NULL) {
            error_at_previous("No parse rule for token");
        }
        if (rule->infix == NULL) {
            error_at_previous("No infix rule for token");
        }
        rule->infix(can_assign);
    }
}

static void expression() {
    parse_precedence(PREC_ASSIGNMENT);
}

static void expression_statement() {
    expression();
    consume(TOKEN_SEMICOLON, "expected semicolon at end of expression");
    emit_byte(OP_POP);
}

static void print_statement() {
    expression();
    consume(TOKEN_SEMICOLON, "expected semicolon at end of print");
    emit_byte(OP_PRINT);
}

static void statement() {
    if (match(TOKEN_PRINT)) {
        print_statement();
    } else {
        expression_statement();
    }
}

static void var_declaration() {
    consume(TOKEN_IDENTIFIER, "expected identifier");
    int constant_index = identifier_constant();
    if (match(TOKEN_EQUAL)) {
        expression();
    } else {
        emit_byte(OP_NIL);
    }
    consume(TOKEN_SEMICOLON, "expected semicolon at end of variable declaration");
    emit_bytes(OP_DEFINE_VARIABLE, constant_index);
}

static void declaration() {
    if (match(TOKEN_VAR)) {
        var_declaration();
    } else {
        statement();
    }
}

static void program() {
    while (!match(TOKEN_EOF)) {
        declaration();
    }
}

bool compile(char *source, lox_chunk *chunk) {
    current_chunk = chunk;
    parser.had_error = false;
    scanner_init(source);
    advance();
    program();
    chunk_add_code(chunk, OP_RETURN, 0);
    return !parser.had_error;
}
