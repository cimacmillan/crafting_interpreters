#pragma once

#include "Expr.h"
#include "LoxValue.h"
#include "Environment.h"

#include <enum.h>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct LoxRuntimeError {
    std::string message;
};

struct LoxReturn {
    LoxValue value;
};

std::ostream & operator<<(std::ostream & os, const LoxRuntimeError & error);

namespace CPPLox {
class Interpreter {
public:
    LoxProgram program;
    Environment environment;
    Environment *currentEnv;
    std::unordered_map<VariableExpression*, int> variable_hops;

public:
    Interpreter(LoxProgram program, Environment env): program(program), environment(env) {}
    void run();
    void setVariableHops(VariableExpression* expr, int hops);
    int getVariableHops(VariableExpression* expr);

};
};

LoxValue evaluate(LiteralExpression* expr, CPPLox::Interpreter *env);
LoxValue evaluate(GroupingExpression* expr, CPPLox::Interpreter *env);
LoxValue evaluate(UnaryExpression* expr, CPPLox::Interpreter *env);
LoxValue evaluate(BinaryExpression* expr, CPPLox::Interpreter *env);
LoxValue evaluate(VariableExpression* expr, CPPLox::Interpreter *env);
LoxValue evaluate(Expression* expr, CPPLox::Interpreter *env);
void evaluate(BlockStatement *block, CPPLox::Interpreter *environment, std::unordered_map<std::string, LoxValue> args);
void evaluate(Statement* declaration, CPPLox::Interpreter *environment);
void evaluate(Declaration* declaration, CPPLox::Interpreter *environment);


