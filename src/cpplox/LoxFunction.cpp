#include "LoxValue.h"
#include "Environment.h"
#include "Interpreter.h"
#include "LoxFunction.h"

LoxNativeFunction::LoxNativeFunction(struct LoxValue (*func)(std::vector<struct LoxValue> arguments)): func(func) {}
LoxValue LoxNativeFunction::call(std::vector<struct LoxValue> arguments) {
    return this->func(arguments);
}

LoxFunction::LoxFunction(FunctionDeclaration *decl, Environment *scope): decl(decl), scope(scope) {

}
struct LoxValue LoxFunction::call(std::vector<struct LoxValue> arguments) {
    Environment env(this->scope);
    auto argLength = arguments.size();
    for (int i = 0; i < argLength; i++) {
        std::string name = this->decl->argIdentifiers->at(i)->lexeme;
        env.defineVariable(name);
        env.setVariable(name, arguments[i]);
    }
    try {
        evaluate(decl->block, &env);
    } catch (LoxReturn rtn) {
        return rtn.value;
    }
    return (LoxValue){.type = LoxValueType::NIL};
}
