#pragma once

#include "Expr.h"
#include "LoxValue.h"
#include "Environment.h"

#include <enum.h>
#include <iostream>
#include <vector>

using namespace std;

struct LoxRuntimeError {
    std::string message;
};

std::ostream & operator<<(std::ostream & os, const LoxRuntimeError & error);

LoxValue evaluate(LiteralExpression* expr, Environment *env);
LoxValue evaluate(GroupingExpression* expr, Environment *env);
LoxValue evaluate(UnaryExpression* expr, Environment *env);
LoxValue evaluate(BinaryExpression* expr, Environment *env);
LoxValue evaluate(VariableExpression* expr, Environment *env);
LoxValue evaluate(Expression* expr, Environment *env);

namespace CPPLox {
class Interpreter {
private:
    LoxProgram program;
    Environment environment;

public:
    Interpreter(LoxProgram program): program(program) {}
    void run();

};
};

