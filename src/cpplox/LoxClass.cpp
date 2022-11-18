#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "LoxValue.h"
#include "Token.h"

#include <sstream>

LoxClass::LoxClass(ClassDeclaration *decl, Environment *scope, Interpreter *env,
                   std::unordered_map<std::string, LoxCallable *> methods)
    : decl(decl), scope(scope), env(env), methods(methods) {}

struct LoxValue LoxClass::call(std::vector<struct LoxValue> arguments) {
  auto instance = new LoxInstance(this);
  auto func = instance->get_member("init");
  if (func.type == +LoxValueType::CALLABLE) {
    func.callable->call(arguments);
  }
  return (LoxValue){.type = LoxValueType::INSTANCE, .instance = instance};
}

std::string LoxClass::to_string() {
  stringstream ss;
  ss << "class ";
  ss << this->decl->identifier->lexeme;
  return ss.str();
}

std::optional<LoxCallable *> LoxClass::find_method(std::string name) {
  auto function = this->methods.find(name);

  if (function == this->methods.end()) {
    return std::nullopt;
  }

  return function->second;
}

LoxCallable *LoxClass::bind(struct LoxInstance *instance) {
  // no-op
  return this;
}