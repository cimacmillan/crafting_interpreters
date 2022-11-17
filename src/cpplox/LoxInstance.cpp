#include "LoxClass.h"
#include "LoxInstance.h"
#include "LoxValue.h"
#include "LoxError.h"

#include <sstream>

LoxInstance::LoxInstance(LoxClass *klass): klass(klass) {

}

std::string LoxInstance::to_string() {
    std::stringstream ss;
    ss << "instance of ";
    ss << klass->to_string();
    return ss.str();
}

LoxValue LoxInstance::get_member(std::string name) {
    if (this->members.find(name) == this->members.end()) {
        std::stringstream ss;
        ss << "There is no member ";
        ss << name;
        ss << " on ";
        ss << this->to_string();
        throw LoxRuntimeError({ .message=ss.str() });
    }

    return this->members.at(name);
}



