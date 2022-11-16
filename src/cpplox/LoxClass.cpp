#include "LoxClass.h"
#include "LoxInstance.h"
#include "LoxValue.h"
#include "Token.h"

#include <sstream>

LoxClass::LoxClass(ClassDeclaration *decl, Environment *scope, Interpreter *env): decl(decl), scope(scope), env(env) {}

struct LoxValue LoxClass::call(std::vector<struct LoxValue> arguments) {
    return (LoxValue){
        .type=LoxValueType::INSTANCE,
        .instance = new LoxInstance(this)
    };
}

std::string LoxClass::to_string() {
    stringstream ss;
    ss << "class ";
    ss << this->decl->identifier->lexeme;
    return ss.str();
}
