#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

namespace CPPLox {

class TokenScanner {

  public:
    TokenScanner(std::string source) : source(source) {}
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

    // Conditional advance, which only advances if the next symbol is c
    // Returns true if that happened, false otherwise
    bool match(char c);

    // Return the value of the next character, don't advance
    char peek();

    // Peek the following symbol
    char peekNext();

    // String has started, keep advancing until meeting quotes
    void advanceString();

    // Number has started, keep advancing until end of number
    void advanceNumber();

    // Advance identifier or keyword
    void advanceIdentifier();
};

} // namespace CPPLox
