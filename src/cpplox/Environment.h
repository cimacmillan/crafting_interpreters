#pragma once

#include "LoxValue.h"
#include "Token.h"

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

class Environment {
  private:
    std::unordered_map<std::string, LoxValue> variables;
    Environment *parent;

  public:
    Environment(Environment *parent,
                std::unordered_map<std::string, LoxValue> args)
        : parent(parent), variables(args) {}
    bool defineVariable(std::string lexeme);
    bool setVariable(std::string lexeme, struct LoxValue value);
    std::optional<LoxValue> getVariable(Token token, int hops);
    void print();
};
