#include "LoxClass.h"
#include "LoxInstance.h"

#include <sstream>

LoxInstance::LoxInstance(LoxClass *klass): klass(klass) {

}

std::string LoxInstance::to_string() {
    std::stringstream ss;
    ss << "instance of ";
    ss << klass->to_string();
    return ss.str();
}



