#pragma once

#include "ast/ast.h"

#include <enum.h>
#include <vector>
#include "Token.h"

struct LoxProgram {
    std::vector<Declaration*> declarations;
};

