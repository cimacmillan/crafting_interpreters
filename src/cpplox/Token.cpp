#include <iostream>
#include <string>

#include "Token.h"

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.lexeme << " " << token.type << " " << token.line;
  return os;
}

std::ostream &operator<<(std::ostream &os, const TokenType &token) {
  os << token._to_string();
  return os;
}
