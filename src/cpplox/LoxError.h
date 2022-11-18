#pragma once

#include "LoxValue.h"
#include <string>

struct LoxRuntimeError {
  std::string message;
};

struct LoxReturn {
  LoxValue value;
};
