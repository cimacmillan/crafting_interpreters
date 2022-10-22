#pragma once

#include "Token.h"

using namespace std;

namespace CPPLox {

    void report(int line, string message);
    void fatal(int line, string message);
    void fatal_token(Token token, string message);
}

