#pragma once

#include "Expr.h"
#include <enum.h>
#include <iostream>
#include <vector>

using namespace std;

BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL);

struct LoxValue {
    LoxValueType type;
    union {
        double number;
        std::string *str;
        bool boolean;
    };
};

std::ostream & operator<<(std::ostream & os, const LoxValue & error);

struct LoxRuntimeError {
    std::string message;
};

std::ostream & operator<<(std::ostream & os, const LoxRuntimeError & error);

LoxValue evaluate(LiteralExpression* expr);
LoxValue evaluate(GroupingExpression* expr);
LoxValue evaluate(UnaryExpression* expr);
LoxValue evaluate(BinaryExpression* expr);
LoxValue evaluate(Expression* expr);

namespace CPPLox {
class Interpreter {
private:
    LoxProgram program;

public:
    Interpreter(LoxProgram program): program(program) {}
    void run();

};
};

