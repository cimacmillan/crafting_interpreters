#include "Interpreter.h"
#include "CPPLox.h"
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

double parseNumber(std::string lexeme) {
    return atof(lexeme.c_str());
}

LoxValue evaluate(LiteralExpression* expr) {
    switch(expr->literal->type) {
        case +TokenType::NUMBER:
            return { .type=LoxValueType::NUMBER, .number=parseNumber(expr->literal->lexeme)};
        case +TokenType::STRING:
            return { .type=LoxValueType::STRING, .str=&(expr->literal->lexeme)};
        case +TokenType::TRUE:
            return { .type=LoxValueType::BOOLEAN, .boolean=true };
        case +TokenType::FALSE:
            return { .type=LoxValueType::BOOLEAN, .boolean=false };
        case +TokenType::NIL:
            return { .type=LoxValueType::NIL };
    }
    CPPLox::fatal_token(*(expr->literal), "Invalid literal, parsing error");
}

LoxValue evaluate(GroupingExpression* expr) {
    return evaluate(expr->grouped);
}

bool isTruthy(LoxValue value) {
    if (value.type == +LoxValueType::NIL) return false;
    if (value.type == +LoxValueType::BOOLEAN) return value.boolean;
    return true;
}

LoxValue evaluate(UnaryExpression* expr) {
    LoxValue value = evaluate(expr->expression);
    switch (expr->unary->type) {
        case TokenType::MINUS:
            if (value.type == +LoxValueType::NUMBER) {
                value.number = -value.number;
                return value;
            }
            break;
        case TokenType::BANG:
            return {LoxValueType::BOOLEAN, .boolean=!isTruthy(value)};
    }

    string message = "Invalid unary operation for value type ";
    message.append(value.type._to_string());
    CPPLox::fatal_token(*(expr->unary), message);
}

bool isDualType(LoxValue a, LoxValue b, LoxValueType type) {
    return (a.type == b.type) && a.type == type;
}

bool isEqual(LoxValue a, LoxValue b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case LoxValueType::NIL:
            return true;
        case LoxValueType::BOOLEAN:
            return a.boolean == b.boolean;
        case LoxValueType::NUMBER:
            return a.number == b.number;
        case LoxValueType::STRING:
            return *(a.str) == *(b.str);
    }
}

LoxValue evaluate(BinaryExpression* expr) {
    LoxValue left = evaluate(expr->left);
    LoxValue right = evaluate(expr->right);
    switch (expr->op->type) {
        case TokenType::MINUS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number - right.number};
            }
        break;
        case TokenType::PLUS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number + right.number};
            }
            if (isDualType(left, right, LoxValueType::STRING)) {
                string* concat = new string(*(left.str));
                concat->append(*(right.str));
                return { .type=LoxValueType::STRING, .str=concat};
            }
        break;
        case TokenType::SLASH:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number / right.number};
            }
        break;
        case TokenType::STAR:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number * right.number};
            }
        break;
        case TokenType::LESS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number < right.number};
            }
        break;
        case TokenType::LESS_EQUAL:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number <= right.number};
            }
        break;
        case TokenType::GREATER:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number > right.number};
            }
        break;
        case TokenType::GREATER_EQUAL:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number >= right.number};
            }
        break;
        case TokenType::EQUAL_EQUAL:
            return { .type=LoxValueType::BOOLEAN, .boolean=isEqual(left, right)};
        break;
        case TokenType::BANG_EQUAL:
            return { .type=LoxValueType::BOOLEAN, .boolean=!isEqual(left, right)};
        break;

    }

    string message = "Invalid binary operation for value type left ";
    message.append(left.type._to_string());
    message.append(" and right ");
    message.append(right.type._to_string());
    CPPLox::fatal_token(*(expr->op), message);
}

LoxValue evaluate(Expression* expr) {
    switch (expr->type) {
        case ExpressionType::LITERAL:
            return evaluate(expr->literal);
        case ExpressionType::GROUPING:
            return evaluate(expr->group);
        case ExpressionType::BINARY:
            return evaluate(expr->binary);
        case ExpressionType::UNARY:
            return evaluate(expr->unary);
    }
}

void CPPLox::Interpreter::run() {
    cout << "Running..." << endl;
    LoxValue value = evaluate(this->expr);
    value.print();
}


