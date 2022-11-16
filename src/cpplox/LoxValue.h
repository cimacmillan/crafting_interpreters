#pragma once

#include <enum.h>
#include <iostream>
#include <vector>
#include <string>

#include "ast/ast.h"
#include "LoxInstance.h"
#include "LoxCallable.h"

BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL, CALLABLE, INSTANCE);

struct LoxValue {
    LoxValueType type;
    union {
        double number;
        std::string *str;
        bool boolean;
        LoxCallable *callable;
        LoxInstance *instance;
    };
};

std::ostream & operator<<(std::ostream & os, const LoxValue & error);

