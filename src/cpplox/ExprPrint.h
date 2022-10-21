#include "Expr.h"

std::string print_expression(LiteralExpression *expression);
std::string print_expression(GroupingExpression *expression);
std::string print_expression(UnaryExpression *expression);
std::string print_expression(BinaryExpression *expression);
std::string print_expression(Expression *expression);
