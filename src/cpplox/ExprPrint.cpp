#include <iostream>
#include <string>

#include "Expr.h"
#include "ExprPrint.h"

using namespace std;

string parenthesize(string name, string contents) {
    string result;
    result.append("(");
    result.append(name);
    result.append(" ");
    result.append(contents);
    result.append(")");
    return result;
}

string quotize(string name) {
    string result;
    result.append("\"");
    result.append(name);
    result.append("\"");
    return result;
}

string print_expression(LiteralExpression *expression) {
    switch (expression->literal->type) {
        case TokenType::STRING:
            return quotize(expression->literal->lexeme);
        default:
            return expression->literal->lexeme;
    }
} 

string print_expression(GroupingExpression *expression) {
    return parenthesize("group", print_expression(expression->grouped));
} 

string print_expression(UnaryExpression *expression) {
    return parenthesize(expression->unary->lexeme, print_expression(expression->expression));
} 

string print_expression(BinaryExpression *expression) {
    string first = print_expression(expression->left);
    string second = print_expression(expression->right);
    first.append(" ");
    first.append(second);
    return parenthesize(expression->op->lexeme, first);
} 

string print_expression(Expression *expression) {
    switch (expression->type) {
        case ExpressionType::LiteralExpression:
            return print_expression(expression->literalexpression);
        case ExpressionType::GroupingExpression:
            return print_expression(expression->groupingexpression);
        case ExpressionType::UnaryExpression:
            return print_expression(expression->unaryexpression);
        case ExpressionType::BinaryExpression:
            return print_expression(expression->binaryexpression);
    }
}
