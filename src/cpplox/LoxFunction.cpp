#include "sstream"

#include "LoxValue.h"
#include "Environment.h"
#include "Interpreter.h"
#include "LoxFunction.h"

LoxNativeFunction::LoxNativeFunction(struct LoxValue (*func)(std::vector<struct LoxValue> arguments), std::string signature): func(func), signature(signature) {}

std::string LoxNativeFunction::to_string() {
    return this->signature;
}

LoxValue LoxNativeFunction::call(std::vector<struct LoxValue> arguments) {
    return this->func(arguments);
}

LoxCallable* LoxNativeFunction::bind(LoxInstance* to) {
    return this;
}

LoxFunction::LoxFunction(FunctionDeclaration *decl, Environment *scope, Interpreter *env): decl(decl), scope(scope), env(env) {

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

std::string LoxFunction::to_string() {
    stringstream ss;
    ss << "fun ";
    ss << this->decl->identifier->lexeme;
    ss << "(";
    bool c_sw = false;
    for (auto ident : *(this->decl->argIdentifiers)) {
        if (!c_sw) {
            ss << ",";
            c_sw = true;
        }
        ss << ident->lexeme;
    }
    ss << ")";
    return ss.str();
}

LoxCallable* LoxFunction::bind(LoxInstance* to) {
    std::unordered_map<std::string, LoxValue> args = {{ "this", (LoxValue){ .type=LoxValueType::INSTANCE, .instance=to}}};
    auto bound_env = new Environment(this->scope, args);
    LoxFunction *newFunc = new LoxFunction(this->decl, bound_env, this->env);
    return newFunc;
}
