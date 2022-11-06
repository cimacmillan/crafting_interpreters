#pragma once

#include <vector>
#include "Token.h"
#include "Expr.h"

using namespace std;


/**
 * expression -> equality
 * equalty => comparison ( != | == comparison ) *
 * comparison -> term ( > | >= | < | <= term)*
 * term -> factor ( - | + factor) *
 * factor -> unary ( / | * unary)*
 * unary -> (( ! | - ) unary) | primary
 * primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" + expression + ")"
*/


namespace CPPLox {
class LoxParser {
private:
    vector<Token> tokens;
    int current;

    Statement* statement();
    Statement* printExpression();
    Statement* statementExpression();
    Expression* assignment();
    Expression* expression();
    Expression* equality();
    Expression* comparison();
    Expression* term();
    Expression* factor();
    Expression* unary();
    Expression* primary();
    Declaration* var_declaration();
    Declaration* statement_declaration();
    Declaration* declaration();

    bool match(TokenType type);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();

public:
    LoxParser(vector<Token> tokens): tokens(tokens), current(0) {

    }

    LoxProgram parse();

};
};
