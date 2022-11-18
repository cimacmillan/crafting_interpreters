#pragma once

#include "Environment.h"
#include "Expr.h"
#include "LoxError.h"
#include "LoxValue.h"

#include <enum.h>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

std::ostream &operator<<(std::ostream &os, const LoxRuntimeError &error);

class Interpreter {
public:
  LoxProgram program;
  Environment environment;
  Environment *currentEnv;
  std::unordered_map<Expression *, int> variable_hops;

public:
  Interpreter(LoxProgram program, Environment env)
      : program(program), environment(env) {}
  void run();
  void setVariableHops(Expression *expr, int hops);
  int getVariableHops(Expression *expr);
};

LoxValue evaluate(LiteralExpression *expr, Interpreter *env);
LoxValue evaluate(GroupingExpression *expr, Interpreter *env);
LoxValue evaluate(UnaryExpression *expr, Interpreter *env);
LoxValue evaluate(BinaryExpression *expr, Interpreter *env);
LoxValue evaluate(VariableExpression *expr, Interpreter *env);
LoxValue evaluate(Expression *expr, Interpreter *env);
void evaluate(BlockStatement *block, Interpreter *environment,
              std::unordered_map<std::string, LoxValue> args);
void evaluate(Statement *declaration, Interpreter *environment);
void evaluate(Declaration *declaration, Interpreter *environment);
