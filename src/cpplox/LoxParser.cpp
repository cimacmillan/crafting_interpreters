
#include <vector>
#include <string>
#include "Token.h"
#include "Expr.h"
#include "LoxParser.h"
#include "CPPLox.h"

using namespace std;

bool CPPLox::LoxParser::match(TokenType type) {
    if (this->check(type)) {
        this->advance();
        return true;
    }
    return false;
}

bool CPPLox::LoxParser::check(TokenType type) {
    return this->peek().type == type;
}

Token CPPLox::LoxParser::advance() {
    Token eaten = this->peek();
    if (this->isAtEnd()) return eaten;
    this->current++;
    return eaten;
}

bool CPPLox::LoxParser::isAtEnd() {
    switch (this->peek().type) {
        case TokenType::FILE_END:
            return true;
        default: 
            return false;
    }
}

Token CPPLox::LoxParser::peek() {
    return this->tokens[this->current];
}

Token CPPLox::LoxParser::previous() {
    return this->tokens[this->current - 1];
}

Expression* CPPLox::LoxParser::primary() {
    if (this->match(TokenType::NUMBER)) {
        Token* previous = new Token(this->previous());
        return Expression::asLiteral(previous);
    }
    if (this->match(TokenType::STRING)) {
        Token* previous = new Token(this->previous());
        return Expression::asLiteral(previous);
    }
    if (this->match(TokenType::TRUE)) {
        Token* previous = new Token(this->previous());
        return Expression::asLiteral(previous);
    }
    if (this->match(TokenType::FALSE)) {
        Token* previous = new Token(this->previous());
        return Expression::asLiteral(previous);
    }
    if (this->match(TokenType::NIL)) {
        Token* previous = new Token(this->previous());
        return Expression::asLiteral(previous);
    }
    if (this->match(TokenType::IDENTIFIER)) {
        Token* previous = new Token(this->previous());
        return Expression::asVariable(previous);
    }

    if (this->match(TokenType::LEFT_PAREN)) {
        Token* left = new Token(this->previous());
        Expression* expr = this->expression();
        if (this->match(TokenType::RIGHT_PAREN)) {
            Token* right = new Token(this->previous());
            return Expression::asGrouping(left, expr, right);
        }
        CPPLox::fatal_token(this->peek(), "expected right parenthesis");
    }

    CPPLox::fatal_token(this->peek(), "Expected expression");
    return nullptr;
}

Expression* CPPLox::LoxParser::unary() {
    if (this->match(TokenType::BANG) || this->match(TokenType::MINUS)) {
        Token *previous = new Token(this->previous());
        Expression* again = this->unary();
        return Expression::asUnary(
            again,
            previous
        );
    }
    return this->primary();
}

Expression* CPPLox::LoxParser::factor() {
    Expression* left = this->unary();
    while (this->match(TokenType::SLASH) || this->match(TokenType::STAR)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->unary();
        left = Expression::asBinary(left, previous, right);
    }
    return left;
}

Expression* CPPLox::LoxParser::term() {
    Expression* left = this->factor();
    while (this->match(TokenType::MINUS) || this->match(TokenType::PLUS)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->factor();
        left = Expression::asBinary(left, previous, right);
    }
    return left;
}

Expression* CPPLox::LoxParser::comparison() {
    Expression* left = this->term();
    while (this->match(TokenType::GREATER) || this->match(TokenType::GREATER_EQUAL) || this->match(TokenType::LESS) || this->match(TokenType::LESS_EQUAL)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->term();
        left = Expression::asBinary(left, previous, right);
    }
    return left;
}

Expression* CPPLox::LoxParser::equality() {
    Expression* left = this->comparison();
    while (this->match(TokenType::BANG_EQUAL) || this->match(TokenType::EQUAL_EQUAL)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->comparison();
        left = Expression::asBinary(left, previous, right);
    }
    return left;
}

Expression* CPPLox::LoxParser::expression() {
    return this->equality();
}

Statement* CPPLox::LoxParser::printExpression() {
    Token* print = new Token(this->previous());
    Expression* expr = this->expression();
    if (!this->match(TokenType::SEMICOLON)) {
        CPPLox::fatal_token(this->peek(), "Expected semicolon");
    }
    Token* semi = new Token(this->previous());
    return Statement::asPrintStatement(print, expr, semi);
}

Statement* CPPLox::LoxParser::statementExpression() {
    Expression* expr = this->expression();
    if (!this->match(TokenType::SEMICOLON)) {
        CPPLox::fatal_token(this->peek(), "Expected semicolon");
    }
    Token* semi = new Token(this->previous());
    return Statement::asExpressionStatement(expr, semi);
}

Statement* CPPLox::LoxParser::statement() {
    if (this->match(TokenType::PRINT)) {
        return this->printExpression();
    }
    return this->statementExpression();
}

Declaration* CPPLox::LoxParser::var_declaration() {
    Token* var = new Token(this->previous());
    
    if (!this->match(TokenType::IDENTIFIER)) {
        CPPLox::fatal_token(this->peek(), "Expected identifier");
    }

    Token* identifier = new Token(this->previous());

    if (this->match(TokenType::EQUAL)) {
        Token* equal = new Token(this->previous());
        Expression* expr = this->expression();
        if (!this->match(TokenType::SEMICOLON)) {
            CPPLox::fatal_token(this->peek(), "Expected semicolon");
        }
        Token* semi = new Token(this->previous());
        return Declaration::asVarDeclaration(var, identifier, equal, expr, semi);
    } else {
        if (!this->match(TokenType::SEMICOLON)) {
            CPPLox::fatal_token(this->peek(), "Expected semicolon");
        }
        Token* semi = new Token(this->previous());
        return Declaration::asVarDeclaration(var, identifier, nullptr, nullptr, semi);
    }
}

Declaration* CPPLox::LoxParser::statement_declaration() {
    return Declaration::asStatementDeclaration(this->statement());
}

Declaration* CPPLox::LoxParser::declaration() {
    if (match(TokenType::VAR)) {
        return this->var_declaration();
    }
    return this->statement_declaration();
}

LoxProgram CPPLox::LoxParser::parse() {
    LoxProgram program;
    while (!this->isAtEnd()) {
        program.declarations.push_back(this->declaration());
    }
    return program;
}
