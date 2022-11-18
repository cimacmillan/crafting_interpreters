#include "LoxInstance.h"
#include "LoxClass.h"
#include "LoxError.h"
#include "LoxValue.h"

#include <sstream>

LoxInstance::LoxInstance(LoxClass *klass) : klass(klass) {}

std::string LoxInstance::to_string() {
    std::stringstream ss;
    ss << "instance of ";
    ss << klass->to_string();
    return ss.str();
}

bool LoxInstance::has_member(std::string name) {
    return this->members.find(name) != this->members.end();
}

LoxValue LoxInstance::get_member(std::string name) {
    if (this->has_member(name)) {
        return this->members.at(name);
    }

    auto func_opt = this->klass->find_method(name);
    if (func_opt.has_value()) {
        LoxCallable *func = func_opt.value()->bind(this);
        return (LoxValue){.type = LoxValueType::CALLABLE, .callable = func};
    }

    std::stringstream ss;
    ss << "There is no member or method ";
    ss << name;
    ss << " on ";
    ss << this->to_string();
    throw LoxRuntimeError({.message = ss.str()});
}

void LoxInstance::set_member(std::string name, LoxValue value) {
    this->members.emplace(name, value);
}
