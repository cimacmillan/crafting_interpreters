#pragma once

// #include "Environment.h"
// #include "Interpreter.h"
#include "LoxCallable.h"
// #include "LoxFunction.h"

#include <unordered_map>
#include <vector>

class LoxClass : public LoxCallable {
  private:
    struct ClassDeclaration *decl;
    struct Environment *scope;
    struct Interpreter *env;
    std::unordered_map<std::string, LoxCallable *> methods;

  public:
    LoxClass(ClassDeclaration *decl, struct Environment *scope,
             struct Interpreter *env,
             std::unordered_map<std::string, LoxCallable *> methods);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
    std::string to_string();
    std::optional<LoxCallable *> find_method(std::string name);
    LoxCallable *bind(struct LoxInstance *instance);
};
