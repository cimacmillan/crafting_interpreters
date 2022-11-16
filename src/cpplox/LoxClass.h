#pragma once

// #include "Environment.h"
// #include "Interpreter.h"
#include "LoxCallable.h"

class LoxClass : public LoxCallable {
private:
    struct ClassDeclaration *decl;
    struct Environment *scope;
    struct Interpreter *env;
public:
    LoxClass(ClassDeclaration *decl, struct Environment *scope, struct Interpreter *env);
    struct LoxValue call(std::vector<struct LoxValue> arguments);
    std::string to_string();
};




