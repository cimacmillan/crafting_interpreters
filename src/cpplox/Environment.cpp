#include "Environment.h"

#include <iostream>
#include <string>

void Environment::print() {
    std::cout << "Environment" << std::endl;
    for (auto x : this->variables) {
        std::cout << "'" << x.first << "'" << " -> " << x.second << std::endl;
    }
}

bool Environment::defineVariable(std::string lexeme) {
    // Variable is already defined, error
    if (this->variables.find(lexeme) != this->variables.end()) {
        return false;
    }
    this->variables.emplace(lexeme, (LoxValue){ .type=LoxValueType::NIL });
    return true;
}

bool Environment::setVariable(std::string lexeme, LoxValue value) {
    // Variable is not defined
    if (this->variables.find(lexeme) == this->variables.end()) {
        if (this->parent) {
            return this->parent->setVariable(lexeme, value);
        }
        return false;
    }
    this->variables.find(lexeme)->second = value;
    return true;
}

std::optional<LoxValue> Environment::getVariable(Token token) {
    if (this->variables.find(token.lexeme) == this->variables.end()) {
        if (this->parent) {
            return this->parent->getVariable(token);
        }
        return std::nullopt;
    }
    return this->variables.at(token.lexeme);
}
