#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "LoxValue.h"
#include "Token.h"

#include <sstream>

LoxClass::LoxClass(ClassDeclaration *decl, Environment *scope, Interpreter *env,
                   std::unordered_map<std::string, LoxCallable *> methods, LoxClass *parent)
    : decl(decl), scope(scope), env(env), methods(methods), parent(parent) {}

struct LoxValue LoxClass::call(std::vector<struct LoxValue> arguments) {
    auto instance = new LoxInstance(this);
    auto method = this->find_method("init");
    if (method) {
        auto bound = method.value()->bind(instance);
        bound->call(arguments);
    }
    return LoxValue({.type = LoxValueType::INSTANCE, .instance = instance});
}

std::string LoxClass::to_string() {
    stringstream ss;
    ss << "class ";
    ss << this->decl->identifier->lexeme;
    return ss.str();
}

std::optional<LoxCallable *> LoxClass::find_method(std::string name) {
    auto function = this->methods.find(name);

    if (function != this->methods.end()) {
        return function->second;
    }

    if (this->parent) {
        return this->parent->find_method(name);
    }

    return std::nullopt;
}

LoxCallable *LoxClass::bind(class LoxInstance *instance) {
    (void)instance;
    // no-op
    return this;
}