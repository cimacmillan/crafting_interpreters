#pragma once

#include "ast/ast.h"

#include <enum.h>
#include <vector>
#include "Token.h"

BETTER_ENUM(UnaryType, char, SUBTRACT, NOT);
BETTER_ENUM(OperatorType, char, EQUAL_TO, NOT_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, ADD, SUBTRACT, MULTIPLY, DIVIDE);
BETTER_ENUM(LiteralType, char, NUMBER, STRING, TRUE, FALSE, NIL);
BETTER_ENUM(ExpressionType, char, LITERAL, UNARY, BINARY, GROUPING, VAR, ASSIGN);

struct Expression {
    ExpressionType type;
    union {
        BinaryExpression *binary;
        LiteralExpression *literal;
        UnaryExpression *unary;
        GroupingExpression *group;
        VariableExpression *variable;
        AssignExpression *assign;
    };

    static Expression* asUnary(
        Expression *expression,
        Token *unary
    ) {
        return new Expression({
            .type = ExpressionType::UNARY,
            .unary = new UnaryExpression({ expression, unary })
        });
    }

    static Expression* asLiteral(
        Token *literal
    ) {
        return new Expression({
            .type = ExpressionType::LITERAL,
            .literal = new LiteralExpression({ literal })
        });
    }

    static Expression* asBinary(
        Expression *left,
        Token *literal,
        Expression *right
    ) {
        return new Expression({
            .type = ExpressionType::BINARY,
            .binary = new BinaryExpression({ left, literal, right })
        });
    }

    static Expression* asGrouping(
        Token *left,
        Expression *grouped,
        Token *right
    ) {
        return new Expression({
            .type = ExpressionType::GROUPING,
            .group = new GroupingExpression({ left, grouped, right })
        });
    }

    static Expression* asVariable(
        Token *variable
    ) {
        return new Expression({
            .type = ExpressionType::VAR,
            .variable = new VariableExpression({ variable })
        });
    }

    static Expression* asAssignment(
        Token* variable,
        Expression *value
    ) {
        return new Expression({
            .type = ExpressionType::ASSIGN,
            .assign = new AssignExpression({
                variable,
                value
            })
        });
    }
};

BETTER_ENUM(StatementType, char, EXPRESSION_STATEMENT, PRINT_STATEMENT);

struct Statement {
    StatementType type;
    union {
        ExpressionStatement *expr;
        PrintStatement *print;
    };

    static Statement* asExpressionStatement(
        Expression* expr,
        Token* semicolon
    ) {
        return new Statement({
            .type = StatementType::EXPRESSION_STATEMENT,
            .expr = new ExpressionStatement({ expr, semicolon })
        });
    }

    static Statement* asPrintStatement(
        Token* print,
        Expression* expr,
        Token* semicolon
    ) {
        return new Statement({
            .type = StatementType::PRINT_STATEMENT,
            .print = new PrintStatement({ print, expr, semicolon })
        });
    }   

};

BETTER_ENUM(DeclarationType, char, VAR_DECLARATION, STATEMENT_DECLARATION);


struct Declaration {
    DeclarationType type;
    union {
        VarDeclaration *var;
        StatementDeclaration *statement;
    };

    static Declaration* asVarDeclaration(
        Token* var,
        Token* identifier,
        Token* equals,
        Expression* expr,
        Token* semi
    ) {
        return new Declaration({
            .type = DeclarationType::VAR_DECLARATION,
            .var = new VarDeclaration({ var, identifier, equals, expr, semi })
        });
    }

    static Declaration* asStatementDeclaration(
        Statement* statement
    ) {
        return new Declaration({
            .type = DeclarationType::STATEMENT_DECLARATION,
            .statement = new StatementDeclaration({ statement })
        });
    }  
};

struct LoxProgram {
    std::vector<Declaration*> declarations;
};

