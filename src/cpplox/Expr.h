#pragma once

#include <enum.h>
#include "Token.h"

BETTER_ENUM(UnaryType, char, SUBTRACT, NOT);
BETTER_ENUM(OperatorType, char, EQUAL_TO, NOT_EQUAL_TO, LESS_THAN, LESS_THAN_OR_EQUAL_TO, GREATER_THAN, GREATER_THAN_OR_EQUAL_TO, ADD, SUBTRACT, MULTIPLY, DIVIDE);
BETTER_ENUM(LiteralType, char, NUMBER, STRING, TRUE, FALSE, NIL);
BETTER_ENUM(ExpressionType, char, LITERAL, UNARY, BINARY, GROUPING);

struct GroupingExpression {
    Token *open;
    struct Expression *grouped;
    Token *close;
};

struct UnaryExpression {
    struct Expression *expression;
    Token *unary;
};

struct BinaryExpression {
    struct Expression *left;
    Token *op;
    struct Expression *right;
};

struct LiteralExpression {
    Token *literal;
};

struct Expression {
    ExpressionType type;
    union {
        BinaryExpression *binary;
        LiteralExpression *literal;
        UnaryExpression *unary;
        GroupingExpression *group;
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
};


