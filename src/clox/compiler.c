#include "common.h"
#include "compiler.h"
#include "token.h"
#include "scanner.h"
#include "chunk.h"

#define LOCAL_MAX 255

struct lox_local {
    lox_token token;
    int depth;
};

typedef struct lox_local lox_local;

typedef struct {
    lox_token previous;
    lox_token current;
    bool had_error;

    int local_depth;
    int local_count;
    lox_local locals[LOCAL_MAX];
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

void init_parser() {
    parser.local_count = 0;
    parser.local_depth = 0;
}

static lox_parse_rule* get_rule(lox_token_type type);
static void parse_precedence(lox_precedence precedence);
static void expression();
static void declaration();
static int emit_jump(lox_op_code op);
static void patch_jump(int location);

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

static bool identifiers_equal(lox_token a, lox_token b) {
    if (a.length != b.length) return false;
    return memcmp(a.start, b.start, a.length) == 0;
}

static int resolve_local(lox_token token) {
    for (int i = parser.local_count - 1; i >= 0; i--) {
        if (identifiers_equal(token, parser.locals[i].token)) {
            return i;
        }
    }
    return -1;
}

static int identifier_constant() {

    lox_heap_object_string *str = new_lox_string((char*)parser.previous.start, parser.previous.length);
    return chunk_add_constant(current_chunk, TO_OBJ((lox_heap_object*)str));
}

static void variable(bool can_assign) {
    int arg = resolve_local(parser.previous);
    int setOp = OP_SET_LOCAL;
    int getOp = OP_GET_LOCAL;
    // It's a global
    if (arg == -1) {
        arg = identifier_constant();
        setOp = OP_SET_GLOBAL;
        getOp = OP_GET_GLOBAL;
    }

    if (!can_assign && match(TOKEN_EQUAL)) {
        error_at_previous("Cannot assign to anything other than variable");
    }

    if (match(TOKEN_EQUAL)) {
        expression();
        emit_bytes(setOp, arg);
    } else {
        emit_bytes(getOp, arg);
    }
}

static void and_op(bool can_assign) {
    (void)can_assign;
    int jump = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    parse_precedence(PREC_AND);
    patch_jump(jump);
}

static void or_op(bool can_assign) {
    (void)can_assign;
    int false_jump = emit_jump(OP_JUMP_IF_FALSE);
    int true_jump = emit_jump(OP_JUMP);
    patch_jump(false_jump);
    emit_byte(OP_POP);
    parse_precedence(PREC_OR);
    patch_jump(true_jump);
}

static uint8_t call_argument_list() {
    uint8_t arg_c = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression();
            arg_c++;
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "expected right parenthesis in function call");
    return arg_c;
}

static void func_call(bool can_assign) {
    (void)can_assign;
    uint8_t arg_count = call_argument_list();
    emit_bytes(OP_CALL, arg_count);
}

lox_parse_rule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, func_call,   PREC_CALL},
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
  [TOKEN_AND]           = {NULL,     and_op,   PREC_AND},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {literal,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     or_op,   PREC_OR},
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

static void begin_scope() {
    parser.local_depth++;
}

static void end_scope() {
    for (int i = parser.local_count - 1; i >= 0; i--) {
        if (parser.locals[i].depth < parser.local_depth) break;
        parser.local_count--;
        emit_byte(OP_POP);
    }
    
    parser.local_depth--;

}

static void block_statement() {
    begin_scope();
    while (!match(TOKEN_EOF) && !match(TOKEN_RIGHT_BRACE)) {
        declaration();
    }
    end_scope();

    if (parser.previous.type == TOKEN_EOF) {
        error_at_previous("Unexpected end of file when should have got right brace");
    }
}

static void return_statement() {
    if (match(TOKEN_SEMICOLON)) {
        emit_bytes(OP_NIL, OP_RETURN);
    } else {
        expression();
        consume(TOKEN_SEMICOLON, "Expected semicolon after return statement");
        emit_byte(OP_RETURN);
    }
}

static void statement() {
    if (match(TOKEN_PRINT)) {
        print_statement();
    } else if (match(TOKEN_LEFT_BRACE)) {
        block_statement();
    } else if (match(TOKEN_RETURN)) {
        return_statement();
    } else {
        expression_statement();
    }
}

static void add_local(lox_token token) {
    parser.locals[parser.local_count] = (lox_local){
        .depth = parser.local_depth,
        .token = token
    };
    parser.local_count++;
}

static void declare_local_var() {
    // Dont keep track of global vars
    if (parser.local_depth == 0) {
        return;
    }

    add_local(parser.previous);
}

static int declare_var() {
    consume(TOKEN_IDENTIFIER, "expected identifier");
    declare_local_var();
    if (parser.local_depth != 0) {
        return 0;
    }
    int constant_index = identifier_constant();
    return constant_index;
}

static void var_declaration() {
    int constant_index = declare_var();
    if (match(TOKEN_EQUAL)) {
        expression();
    } else {
        emit_byte(OP_NIL);
    }
    consume(TOKEN_SEMICOLON, "expected semicolon at end of variable declaration");
    
    // We only define globals in the global map, otherwise we can skip it because it's read from the stack instead
    if (parser.local_depth == 0) {
        emit_bytes(OP_DEFINE_GLOBAL, constant_index);
    }
}

static int emit_jump(lox_op_code op) {
    emit_byte(op);
    emit_byte(0xFF);
    emit_byte(0xFF);
    return current_chunk->bytecode.size - 2;
}

static void patch_jump(int location) {
    int jump = current_chunk->bytecode.size - location - 2;
    current_chunk->bytecode.code[location] = (jump >> 8) & 0xFF;
    current_chunk->bytecode.code[location + 1] = jump & 0xFF;
}

static void emit_loop(int pos) {
    emit_byte(OP_LOOP);
    int offset = current_chunk->bytecode.size - pos + 2;
    emit_byte((offset >> 8) & 0xFF);
    emit_byte(offset & 0xFF);
}

static void if_declaration() {
    consume(TOKEN_LEFT_PAREN, "expected opening brace on if statement");
    expression();
    consume(TOKEN_RIGHT_PAREN, "expected closing brace on if statement");
    int jump = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    statement();
    int jump_over_else = emit_jump(OP_JUMP);
    patch_jump(jump);
    emit_byte(OP_POP);
    if (match(TOKEN_ELSE)) {
        statement();
    }
    patch_jump(jump_over_else);
}

static void while_declaration() {
    int loop_point = current_chunk->bytecode.size;
    consume(TOKEN_LEFT_PAREN, "expected opening brace for while");
    expression();
    consume(TOKEN_RIGHT_PAREN, "expected closing brace for while");
    int skip = emit_jump(OP_JUMP_IF_FALSE);
    emit_byte(OP_POP);
    statement();
    emit_loop(loop_point);
    patch_jump(skip);
    emit_byte(OP_POP);
}

static void for_declaration() {
    begin_scope();
    consume(TOKEN_LEFT_PAREN, "expected left parenthesis on for loop");
    if (match(TOKEN_SEMICOLON)) {
        // no init
    } else if (match(TOKEN_VAR)) {
        var_declaration();
    } else {
        expression_statement();
    }
    // Loop start
    int loop_start = current_chunk->bytecode.size;
    int exit_jump = -1;
    if (!match(TOKEN_SEMICOLON)) {
        expression();
        consume(TOKEN_SEMICOLON, "expected semicolon after expr in for");
        exit_jump = emit_jump(OP_JUMP_IF_FALSE);
        emit_byte(OP_POP);
    }
    
    // Skip over initializer then loop back later
    if (!match(TOKEN_RIGHT_PAREN)) {
        int body_jump = emit_jump(OP_JUMP);
        int increment_start = current_chunk->bytecode.size;
        expression();
        emit_byte(OP_POP);
        consume(TOKEN_RIGHT_PAREN, "expected right parenthesis on for loop");
        emit_loop(loop_start);
        loop_start = increment_start;
        patch_jump(body_jump);
    }

    statement();

    emit_loop(loop_start);

    if (exit_jump != -1) {
        patch_jump(exit_jump);
        emit_byte(OP_POP);
    }

    end_scope();
}

static int argument_list() {
    int arity = 0;
    while(!match(TOKEN_RIGHT_PAREN) && !match(TOKEN_EOF)) {
        declare_var();
        arity ++;

        if (!check(TOKEN_RIGHT_PAREN)) {
            consume(TOKEN_COMMA, "expected comma between arguments");
        }
    }

    if (parser.previous.type == TOKEN_EOF) {
        error_at_previous("got EOF when was expected end of arguments list");
    }

    return arity;
}

static void function_declaration() {
    int constant_index = declare_var();
    char_array name;
    char_array_init(&name);
    for (int i = 0; i < parser.previous.length; i++) {
        char_array_add(&name, parser.previous.start[i]);
    } 

    begin_scope();
    consume(TOKEN_LEFT_PAREN, "expected left parenthesis");
    int arity = argument_list();
    
    consume(TOKEN_LEFT_BRACE, "expected left brace at beginning of function");

    lox_heap_object_function *func = new_lox_function(name);
    func->arity = arity;
    lox_chunk *previous = current_chunk;
    current_chunk = &func->chunk;
    block_statement();
    emit_bytes(OP_NIL, OP_RETURN);
    end_scope();
    current_chunk = previous;

    emit_constant(TO_OBJ((lox_heap_object*)func));

    // We only define globals in the global map, otherwise we can skip it because it's read from the stack instead
    if (parser.local_depth == 0) {
        emit_bytes(OP_DEFINE_GLOBAL, constant_index);
    }
}

static void declaration() {
    if (match(TOKEN_VAR)) {
        var_declaration();
    } else if (match(TOKEN_IF)) {
        if_declaration();
    } else if (match(TOKEN_WHILE)) {
        while_declaration();
    } else if (match(TOKEN_FOR)) {
        for_declaration();
    } else if (match(TOKEN_FUN)) {
        function_declaration();
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
    init_parser();
    current_chunk = chunk;
    parser.had_error = false;
    scanner_init(source);
    advance();
    program();
    chunk_add_code(chunk, OP_RETURN, 0);
    return !parser.had_error;
}
