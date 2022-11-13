#pragma once

#include <enum.h>
#include <iostream>
#include <vector>
#include "ast/ast.h"

class LoxCallable 
{
public:
    virtual struct LoxValue call(std::vector<struct LoxValue> arguments) = 0;
};


BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL, CALLABLE);

struct LoxValue {
    LoxValueType type;
    union {
        double number;
        std::string *str;
        bool boolean;
        LoxCallable *callable;
    };
};

std::ostream & operator<<(std::ostream & os, const LoxValue & error);

