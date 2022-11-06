#pragma once

#include <enum.h>
#include <iostream>

BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL);

struct LoxValue {
    LoxValueType type;
    union {
        double number;
        std::string *str;
        bool boolean;
    };
};

std::ostream & operator<<(std::ostream & os, const LoxValue & error);

