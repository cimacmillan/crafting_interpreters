#include <string>
#include <iostream>
#include <enum.h>

BETTER_ENUM(TokenType, char, 
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    FILE_END
)

struct Token {
    std::string lexeme;
    TokenType type;
    unsigned int line;
};

std::ostream & operator<<(std::ostream & os, const Token & token);
std::ostream & operator<<(std::ostream & os, const TokenType & token);
