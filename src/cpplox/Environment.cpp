#include "Environment.h"

#include <iostream>

void Environment::print() {
    std::cout << "Environment" << std::endl;
    for (auto x : this->variables) {
        std::cout << "'" << x.first << "'" << " -> " << x.second << std::endl;
    }
}

void Environment::setVariable(Token token, LoxValue value) {
    this->variables.emplace(token.lexeme, value);
}

std::optional<LoxValue> Environment::getVariable(Token token) {
    if (this->variables.find(token.lexeme) == this->variables.end()) {
        return std::nullopt;
    }
    return this->variables.at(token.lexeme);
}

