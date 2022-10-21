#include <iostream>

#include "Expr.h"

using namespace std;

void print_expression(LiteralExpression expression) {
    switch (expression.type) {
        case LiteralType::STRING:
            cout << expression.string->lexeme << endl;
            break;
    }
} 

void print_expression(Expression expression) {
    switch (expression.type) {
        case ExpressionType::LITERAL:
            print_expression(*(expression.literal));
            break;
    }
}
