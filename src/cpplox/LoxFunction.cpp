#include "LoxValue.h"
#include "Environment.h"
#include "Interpreter.h"
#include "LoxFunction.h"

LoxNativeFunction::LoxNativeFunction(struct LoxValue (*func)(std::vector<struct LoxValue> arguments)): func(func) {}
LoxValue LoxNativeFunction::call(std::vector<struct LoxValue> arguments) {
    return this->func(arguments);
}

LoxFunction::LoxFunction(FunctionDeclaration *decl, Environment *scope, CPPLox::Interpreter *env): decl(decl), scope(scope), env(env) {

}
struct LoxValue LoxFunction::call(std::vector<struct LoxValue> arguments) {
    auto args = std::unordered_map<std::string, LoxValue> ();
    auto argLength = arguments.size();
    for (int i = 0; i < argLength; i++) {
        std::string name = this->decl->argIdentifiers->at(i)->lexeme;
        args.emplace(name, arguments[i]);
    }
    auto previous = this->env->currentEnv;
    try {
        this->env->currentEnv = this->scope;
        evaluate(decl->block->blockstatement, this->env, args);
        this->env->currentEnv = previous;
    } catch (LoxReturn rtn) {
        this->env->currentEnv = previous;
        return rtn.value;
    }
    return (LoxValue){.type = LoxValueType::NIL};
}
