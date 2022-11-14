#pragma once

#include <vector>

#include "LoxValue.h"
#include "Environment.h"
#include "Interpreter.h"

class LoxNativeFunction : public LoxCallable {
private:
    struct LoxValue (*func)(std::vector<struct LoxValue> arguments);
public:
    LoxNativeFunction(struct LoxValue (*func)(std::vector<struct LoxValue> arguments));
    struct LoxValue call(std::vector<struct LoxValue> arguments);
};

class LoxFunction : public LoxCallable {
private:
    FunctionDeclaration *decl;
    Environment *scope;
    CPPLox::Interpreter *env;
public:
    LoxFunction(FunctionDeclaration *decl, Environment *scope, CPPLox::Interpreter *env);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
};
