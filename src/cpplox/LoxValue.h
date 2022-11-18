#pragma once

#include <enum.h>
#include <iostream>
#include <string>
#include <vector>

#include "LoxCallable.h"
#include "LoxInstance.h"
#include "ast/ast.h"

BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL, CALLABLE,
            INSTANCE);

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

std::ostream &operator<<(std::ostream &os, const LoxValue &error);
