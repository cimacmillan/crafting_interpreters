#include <vector>
#include <string>

#include "Token.h"

namespace CPPLox {
    std::vector<Token> scanTokens(std::string source) {
        std::vector<Token> tokens = { (Token){"Nothing", TokenType::NUMBER, 9} };

        return tokens;
    }
}
