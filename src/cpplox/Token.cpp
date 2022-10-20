#include <string>
#include <iostream>

#include "Token.h"

std::ostream & operator<<(std::ostream & os, const Token & token)
{
   os << token.lexeme << " " << token.type << " " << token.line;
   return os;
}

std::ostream & operator<<(std::ostream & os, const TokenType & token) {
    switch(token) {
        case TokenType::LEFT_PAREN:
            os << "LEFT PARENTHESIS";
            break;
        default:
            os << "UNKNOWN";
            break;
    }

    return os;
}

