#include "LoxValue.h"
#include "Token.h"

#include <unordered_map>
#include <string>
#include <optional>

class Environment {
private:
    std::unordered_map<std::string, LoxValue> variables;
    Environment *parent;

public:
    Environment(Environment *parent) : parent(parent) {}
    bool defineVariable(Token token);
    bool setVariable(Token token, LoxValue value);
    std::optional<LoxValue> getVariable(Token token);
    void print();

};
