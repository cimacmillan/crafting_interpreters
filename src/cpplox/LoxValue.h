#pragma once

#include <enum.h>
#include <iostream>
#include <vector>

struct LoxCallable 
{
    struct LoxValue (*func)(std::vector<struct LoxValue> arguments);
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

