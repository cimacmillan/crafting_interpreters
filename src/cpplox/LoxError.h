#pragma once

#include <string>
#include "LoxValue.h"

struct LoxRuntimeError {
    std::string message;
};

struct LoxReturn {
    LoxValue value;
};
