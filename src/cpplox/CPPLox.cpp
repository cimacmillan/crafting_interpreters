#include <string>
#include <iostream>

#include "CPPLox.h"
#include "Token.h"

using namespace std;

void CPPLox::report(int line, string message) {
    cout << "[line " << line << "] Error: " << message << endl;
}

void CPPLox::fatal(int line, string message) {
    CPPLox::report(line, message);
    exit(1);
}

void CPPLox::fatal_token(Token token, string message) {
    message.append(" - for Token '");
    message.append(token.type == +TokenType::FILE_END ? "EOF" : token.lexeme);
    message.append("'");
    CPPLox::fatal(token.line, message);
}


