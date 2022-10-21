#include <vector>
#include <string>

#include "Token.h"

namespace CPPLox {

class TokenScanner {
public:
    TokenScanner(std::string source): source(source) {}
    std::vector<Token> scanTokens();  

private:
    std::string source;
    std::vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;

    void scanToken();
    bool isAtEnd();

    // Lock the current read range into a lexeme with this type
    void lockToken(TokenType type);

    // Move the read to the next character, return the current character
    // |AB -> A|B
    char advance();
};  

}

