#pragma once

#include <vector>
#include <string>

class LoxCallable 
{
public:
    virtual struct LoxValue call(std::vector<struct LoxValue> arguments) = 0;
    virtual std::string to_string() = 0;
};

