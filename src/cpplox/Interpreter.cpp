#include "Interpreter.h"
#include "CPPLox.h"
#include "LoxClass.h"
#include "LoxFunction.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace std;

void runtimeError(std::string message) {
    throw LoxRuntimeError({.message = message});
}

void Interpreter::setVariableHops(Expression *expr, int hops) {
    this->variable_hops[expr] = hops;
}

int Interpreter::getVariableHops(Expression *expr) {
    return this->variable_hops[expr];
}

std::ostream &operator<<(std::ostream &os, const LoxRuntimeError &error) {
    os << "LoxRuntimeError: " << error.message;
    return os;
}

double parseNumber(std::string lexeme) { return atof(lexeme.c_str()); }

LoxValue evaluate(Expression *parent, VariableExpression *expr,
                  Interpreter *env) {
    auto value = env->currentEnv->getVariable(*(expr->variable),
                                              env->getVariableHops(parent));
    if (!value.has_value()) {
        std::stringstream ss;
        ss << "Variable " << expr->variable->lexeme << " is not defined";
        runtimeError(ss.str());
    }
    return value.value();
}

LoxValue evaluate(Expression *parent, ThisExpression *expr, Interpreter *env) {
    auto value = env->currentEnv->getVariable(*(expr->this_t),
                                              env->getVariableHops(parent));
    if (!value.has_value()) {
        std::stringstream ss;
        ss << "Variable " << expr->this_t->lexeme << " is not defined";
        runtimeError(ss.str());
    }
    return value.value();
}

LoxValue evaluate(LiteralExpression *expr, Interpreter *env) {
    (void)env;
    switch (expr->literal->type) {
    case +TokenType::NUMBER:
        return {.type = LoxValueType::NUMBER,
                .number = parseNumber(expr->literal->lexeme)};
    case +TokenType::STRING:
        return {.type = LoxValueType::STRING, .str = &(expr->literal->lexeme)};
    case +TokenType::TRUE:
        return {.type = LoxValueType::BOOLEAN, .boolean = true};
    case +TokenType::FALSE:
        return {.type = LoxValueType::BOOLEAN, .boolean = false};
    case +TokenType::NIL:
        return {.type = LoxValueType::NIL};
    default:
        runtimeError("Invalid literal type");
        break;
    }
    return LoxValue({.type = LoxValueType::NIL});
}

LoxValue evaluate(GroupingExpression *expr, Interpreter *environment) {
    return evaluate(expr->grouped, environment);
}

bool isTruthy(LoxValue value) {
    if (value.type == +LoxValueType::NIL)
        return false;
    if (value.type == +LoxValueType::BOOLEAN)
        return value.boolean;
    return true;
}

LoxValue evaluate(UnaryExpression *expr, Interpreter *environment) {
    LoxValue value = evaluate(expr->expression, environment);
    if (expr->unary->type == +TokenType::MINUS) {
        if (value.type == +LoxValueType::NUMBER) {
            value.number = -value.number;
            return value;
        }
    } else if (expr->unary->type == +TokenType::BANG) {
        return {LoxValueType::BOOLEAN, .boolean = !isTruthy(value)};
    }

    runtimeError("Invalid unary operation for value type ");
    return LoxValue({.type = LoxValueType::NIL});
}

bool isDualType(LoxValue a, LoxValue b, LoxValueType type) {
    return (a.type == b.type) && a.type == type;
}

bool isEqual(LoxValue a, LoxValue b) {
    if (a.type != b.type)
        return false;
    switch (a.type) {
    case LoxValueType::NIL:
        return true;
    case LoxValueType::BOOLEAN:
        return a.boolean == b.boolean;
    case LoxValueType::NUMBER:
        return a.number == b.number;
    case LoxValueType::STRING:
        return *(a.str) == *(b.str);
    case LoxValueType::CALLABLE:
        return false;
    case LoxValueType::INSTANCE:
        return a.instance == b.instance;
    }
}

LoxValue evaluate(BinaryExpression *expr, Interpreter *environment) {
    LoxValue left = evaluate(expr->left, environment);
    LoxValue right = evaluate(expr->right, environment);
    switch (expr->op->type) {
    case TokenType::MINUS:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::NUMBER,
                    .number = left.number - right.number};
        }
        break;
    case TokenType::PLUS:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::NUMBER,
                    .number = left.number + right.number};
        }
        if (isDualType(left, right, LoxValueType::STRING)) {
            string *concat = new string(*(left.str));
            concat->append(*(right.str));
            return {.type = LoxValueType::STRING, .str = concat};
        }
        break;
    case TokenType::SLASH:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::NUMBER,
                    .number = left.number / right.number};
        }
        break;
    case TokenType::STAR:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::NUMBER,
                    .number = left.number * right.number};
        }
        break;
    case TokenType::LESS:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::BOOLEAN,
                    .boolean = left.number < right.number};
        }
        break;
    case TokenType::LESS_EQUAL:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::BOOLEAN,
                    .boolean = left.number <= right.number};
        }
        break;
    case TokenType::GREATER:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::BOOLEAN,
                    .boolean = left.number > right.number};
        }
        break;
    case TokenType::GREATER_EQUAL:
        if (isDualType(left, right, LoxValueType::NUMBER)) {
            return {.type = LoxValueType::BOOLEAN,
                    .boolean = left.number >= right.number};
        }
        break;
    case TokenType::EQUAL_EQUAL:
        return {.type = LoxValueType::BOOLEAN, .boolean = isEqual(left, right)};
        break;
    case TokenType::BANG_EQUAL:
        return {.type = LoxValueType::BOOLEAN,
                .boolean = !isEqual(left, right)};
        break;
    case TokenType::LEFT_PAREN:
    case TokenType::RIGHT_PAREN:
    case TokenType::LEFT_BRACE:
    case TokenType::RIGHT_BRACE:
    case TokenType::COMMA:
    case TokenType::SEMICOLON:
    case TokenType::DOT:
    case TokenType::BANG:
    case TokenType::EQUAL:
    case TokenType::IDENTIFIER:
    case TokenType::STRING:
    case TokenType::NUMBER:
    case TokenType::AND:
    case TokenType::CLASS:
    case TokenType::ELSE:
    case TokenType::FUN:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::NIL:
    case TokenType::OR:
    case TokenType::PRINT:
    case TokenType::FALSE:
    case TokenType::RETURN:
    case TokenType::SUPER:
    case TokenType::THIS:
    case TokenType::TRUE:
    case TokenType::VAR:
    case TokenType::WHILE:
    case TokenType::FILE_END:
        break;
    }

    runtimeError("Invalid binary operation for value type");
    return LoxValue({.type = LoxValueType::NIL});
}

LoxValue evaluate(AssignExpression *expr, Interpreter *env) {
    LoxValue value = evaluate(expr->value, env);
    if (!env->currentEnv->setVariable(expr->variable->lexeme, value)) {
        stringstream ss;
        ss << "Variable " << (expr->variable->lexeme) << " is undefined";
        runtimeError(ss.str());
    }
    return value;
}

LoxValue evaluate(LogicalExpression *expr, Interpreter *env) {
    LoxValue left = evaluate(expr->left, env);

    if (expr->op->type == +TokenType::OR) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }

    return evaluate(expr->right, env);
}

LoxValue evaluate(CallExpression *expr, Interpreter *environment) {
    LoxValue callee = evaluate(expr->callee, environment);
    if (callee.type != +LoxValueType::CALLABLE) {
        stringstream ss;
        ss << "Callee ";
        ss << callee;
        ss << " is not callable";
        runtimeError(ss.str());
    }
    std::vector<LoxValue> values;
    for (auto arg_expr : *(expr->arguments)) {
        values.push_back(evaluate(arg_expr, environment));
    }
    return callee.callable->call(values);
}

LoxValue evaluate(GetExpression *expr, Interpreter *environment) {
    LoxValue target = evaluate(expr->target, environment);
    if (target.type != +LoxValueType::INSTANCE) {
        stringstream ss;
        ss << "Member target ";
        ss << target;
        ss << " is not an instance of a class. Cannot get ";
        ss << expr->value;
        ss << " from it";
        runtimeError(ss.str());
    }
    return target.instance->get_member(expr->value->lexeme);
}

LoxValue evaluate(SetExpression *expr, Interpreter *environment) {
    if (expr->variable->type != +ExpressionType::GetExpression) {
        stringstream ss;
        ss << "Cannot set value on a non member";
        runtimeError(ss.str());
    }

    LoxValue target =
        evaluate(expr->variable->getexpression->target, environment);
    if (target.type != +LoxValueType::INSTANCE) {
        stringstream ss;
        ss << "Member target ";
        ss << target;
        ss << " is not an instance of a class. Cannot set a member on it.";
        runtimeError(ss.str());
    }

    LoxValue value = evaluate(expr->value, environment);
    target.instance->set_member(expr->variable->getexpression->value->lexeme,
                                value);

    return value;
}

LoxValue evaluate(Expression *parent, SuperExpression *expr, Interpreter *env) {
    auto hops = env->getVariableHops(parent);
    auto value = env->currentEnv->getVariable(*(expr->super_t), hops);
    if (!value.has_value()) {
        stringstream ss;
        ss << "No class bound to super " << *(expr->super_t);
        runtimeError(ss.str());
    }

    auto parent_class = dynamic_cast<LoxClass*>(value->callable);
    if (!parent_class) {
        stringstream ss;
        ss << "No parent class exists at token " << *(expr->super_t);
        runtimeError(ss.str());
    }

    Token token = *(expr->super_t);
    token.lexeme = "this";
    auto lox_instance = env->currentEnv->getVariable(token, hops - 1);

    if (!lox_instance.has_value()) {
        stringstream ss;
        ss << "No this keyword bound at super call " << *(expr->super_t);
        runtimeError(ss.str());
    }

    if (lox_instance->type != +LoxValueType::INSTANCE) {
        stringstream ss;
        ss << "No instance bound to this keyword bound at super call " << *(expr->super_t);
        runtimeError(ss.str());
    }

    LoxInstance *instance_obj = lox_instance->instance;
    auto maybe_method = parent_class->find_method(expr->access->lexeme);

    if (!maybe_method.has_value()) {
        stringstream ss;
        ss << "Superclass has no method " << *(expr->access);
        runtimeError(ss.str());
    }

    LoxCallable *method = maybe_method.value();
    method = method->bind(instance_obj);

    return LoxValue({ .type = LoxValueType::CALLABLE, .callable=method });
}

LoxValue evaluate(Expression *expr, Interpreter *environment) {
    switch (expr->type) {
    case ExpressionType::LiteralExpression:
        return evaluate(expr->literalexpression, environment);
    case ExpressionType::GroupingExpression:
        return evaluate(expr->groupingexpression, environment);
    case ExpressionType::BinaryExpression:
        return evaluate(expr->binaryexpression, environment);
    case ExpressionType::UnaryExpression:
        return evaluate(expr->unaryexpression, environment);
    case ExpressionType::VariableExpression:
        return evaluate(expr, expr->variableexpression, environment);
    case ExpressionType::AssignExpression:
        return evaluate(expr->assignexpression, environment);
    case ExpressionType::LogicalExpression:
        return evaluate(expr->logicalexpression, environment);
    case ExpressionType::CallExpression:
        return evaluate(expr->callexpression, environment);
    case ExpressionType::GetExpression:
        return evaluate(expr->getexpression, environment);
    case ExpressionType::SetExpression:
        return evaluate(expr->setexpression, environment);
    case ExpressionType::ThisExpression:
        return evaluate(expr, expr->thisexpression, environment);
    case ExpressionType::SuperExpression:
        return evaluate(expr, expr->superexpression, environment);
    }
    runtimeError("Unknown expression type");
}

void evaluate(BlockStatement *block, Interpreter *environment,
              std::unordered_map<std::string, LoxValue> args) {
    Environment child(environment->currentEnv, args);
    auto temp = environment->currentEnv;
    environment->currentEnv = &child;
    for (auto statement : *(block->block)) {
        evaluate(statement, environment);
    }
    environment->currentEnv = temp;
}

void evaluate(IfStatement *ifStatement, Interpreter *environment) {
    bool result = isTruthy(evaluate(ifStatement->condition, environment));
    if (result) {
        evaluate(ifStatement->trueBlock, environment);
    } else if (ifStatement->falseBlock != nullptr) {
        evaluate(ifStatement->falseBlock, environment);
    }
}

void evaluate(WhileStatement *whileStatement, Interpreter *environment) {
    while (isTruthy(evaluate(whileStatement->condition, environment))) {
        evaluate(whileStatement->block, environment);
    }
}

void evaluate(ReturnStatement *returnStatement, Interpreter *environment) {
    if (returnStatement->expr != nullptr) {
        LoxValue value = evaluate(returnStatement->expr, environment);
        throw LoxReturn({value});
    } else {
        throw LoxReturn({{.type = LoxValueType::NIL}});
    }
}

void evaluate(Statement *statement, Interpreter *environment) {
    switch (statement->type) {
    case StatementType::ExpressionStatement:
        evaluate(statement->expressionstatement->expr, environment);
        break;
    case StatementType::PrintStatement:
        cout << evaluate(statement->printstatement->expr, environment) << endl;
        break;
    case StatementType::BlockStatement:
        evaluate(statement->blockstatement, environment, {});
        break;
    case StatementType::IfStatement:
        evaluate(statement->ifstatement, environment);
        break;
    case StatementType::WhileStatement:
        evaluate(statement->whilestatement, environment);
        break;
    case StatementType::ReturnStatement:
        evaluate(statement->returnstatement, environment);
        break;
    }
}

void evaluate(VarDeclaration *varDeclaration, Interpreter *environment) {
    LoxValue evaluated = varDeclaration->expr == nullptr
                             ? LoxValue({.type = LoxValueType::NIL})
                             : evaluate(varDeclaration->expr, environment);
    auto defineResult = environment->currentEnv->defineVariable(
        varDeclaration->identifier->lexeme);
    if (!defineResult) {
        std::stringstream error;
        error << "Variable " << varDeclaration->identifier->lexeme
              << " is already defined";
        runtimeError(error.str());
    }
    environment->currentEnv->setVariable(varDeclaration->identifier->lexeme,
                                         evaluated);
}

void evaluate(StatementDeclaration *statementDeclaration,
              Interpreter *environment) {
    evaluate(statementDeclaration->statement, environment);
}

void evaluate(FunctionDeclaration *functionDeclaration,
              Interpreter *environment) {
    // LoxCallable callable
    auto function = new LoxFunction(functionDeclaration,
                                    environment->currentEnv, environment);
    environment->currentEnv->defineVariable(
        functionDeclaration->identifier->lexeme);
    environment->currentEnv->setVariable(
        functionDeclaration->identifier->lexeme,
        LoxValue({.type = LoxValueType::CALLABLE, .callable = function}));
}

void evaluate(ClassDeclaration *classDeclaration, Interpreter *environment) {
    // LoxCallable callable

    LoxClass *parent = nullptr;
    if (classDeclaration->parent) {
        auto value = environment->currentEnv->getVariable(*(classDeclaration->parent->variableexpression->variable),
                                            environment->getVariableHops(classDeclaration->parent));
        // TODO replace with specific LoxClass value type
        parent = dynamic_cast<LoxClass*>(value->callable);
    }

    auto env = environment->currentEnv;
    if (classDeclaration->parent) {
        env = new Environment(env, {{ "super", LoxValue({.type=LoxValueType::CALLABLE, .callable=parent })}});
    }

    std::unordered_map<std::string, LoxCallable *> methods;
    for (auto method : *(classDeclaration->methods)) {
        methods.emplace(
            method->identifier->lexeme,
            new LoxFunction(method, env, environment));
    }

    auto classDecl = new LoxClass(classDeclaration, env,
                                  environment, methods, parent);
    environment->currentEnv->defineVariable(
        classDeclaration->identifier->lexeme);
    environment->currentEnv->setVariable(
        classDeclaration->identifier->lexeme,
        LoxValue({.type = LoxValueType::CALLABLE, .callable = classDecl}));
}

void evaluate(Declaration *declaration, Interpreter *environment) {
    switch (declaration->type) {
    case DeclarationType::VarDeclaration:
        evaluate(declaration->vardeclaration, environment);
        break;
    case DeclarationType::StatementDeclaration:
        evaluate(declaration->statementdeclaration, environment);
        break;
    case DeclarationType::FunctionDeclaration:
        evaluate(declaration->functiondeclaration, environment);
        break;
    case DeclarationType::ClassDeclaration:
        evaluate(declaration->classdeclaration, environment);
        break;
    }
}

void Interpreter::run() {
    this->currentEnv = &(this->environment);
    for (Declaration *declaration : *(this->program.program)) {
        evaluate(declaration, this);
    }
}
