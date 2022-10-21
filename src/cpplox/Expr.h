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
    UnaryType type;
    struct Expression *expression;
    union value {
        Token *subtract;
        Token *NOT;
    };
};

struct BinaryExpression {
    struct Expression *left;
    Token *op;
    struct Expression *right;
};

struct LiteralExpression {
    LiteralType type;
    union {
        Token *number;
        Token *string;
        Token *TRUE;
        Token *FALSE;
        Token *nill;
    };
};

struct Expression {
    ExpressionType type;
    union {
        BinaryExpression *binary;
        LiteralExpression *literal;
        UnaryExpression *unary;
        GroupingExpression *group;
    };
};


