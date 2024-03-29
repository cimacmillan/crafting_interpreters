#pragma once

#include <unordered_map>

#include "LoxClass.h"

struct LoxValue;

class LoxInstance {
  private:
    LoxClass *klass;
    std::unordered_map<std::string, LoxValue> members;

  public:
    LoxInstance(LoxClass *klass);
    std::string to_string();
    bool has_member(std::string name);
    LoxValue get_member(std::string name);
    void set_member(std::string name, LoxValue value);
};
