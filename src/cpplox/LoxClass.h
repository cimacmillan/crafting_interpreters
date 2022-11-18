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
    class Environment *scope;
    class Interpreter *env;
    std::unordered_map<std::string, LoxCallable *> methods;
    LoxClass *parent;

  public:
    LoxClass(ClassDeclaration *decl, class Environment *scope,
             class Interpreter *env,
             std::unordered_map<std::string, LoxCallable *> methods, LoxClass *parent);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
    std::string to_string();
    std::optional<LoxCallable *> find_method(std::string name);
    LoxCallable *bind(class LoxInstance *instance);
};
