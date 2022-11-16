#pragma once

#include "LoxClass.h"

class LoxInstance {
private:
    LoxClass *klass;

public:
    LoxInstance(LoxClass *klass);
    std::string to_string();
};


