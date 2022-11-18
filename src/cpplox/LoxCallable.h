#pragma once

#include <string>
#include <vector>

class LoxCallable {
  public:
    virtual struct LoxValue call(std::vector<struct LoxValue> arguments) = 0;
    virtual std::string to_string() = 0;
    virtual LoxCallable *bind(struct LoxInstance *instance) = 0;
};
