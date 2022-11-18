#pragma once

#include <vector>

#include "Environment.h"
#include "Interpreter.h"
#include "LoxCallable.h"

class LoxNativeFunction : public LoxCallable {
  private:
    struct LoxValue (*func)(std::vector<struct LoxValue> arguments);
    std::string signature;

  public:
    LoxNativeFunction(
        struct LoxValue (*func)(std::vector<struct LoxValue> arguments),
        std::string signature);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
    std::string to_string();
    LoxCallable *bind(class LoxInstance *instance);
};

class LoxFunction : public LoxCallable {
  private:
    FunctionDeclaration *decl;
    Environment *scope;
    Interpreter *env;

  public:
    LoxFunction(FunctionDeclaration *decl, Environment *scope,
                Interpreter *env);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
    std::string to_string();
    LoxCallable *bind(class LoxInstance *instance);
};
