#include "Interpreter.h"
#include "CPPLox.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

void runtimeError(std::string message) {
    throw LoxRuntimeError({ .message=message });
}

std::ostream & operator<<(std::ostream & os, const LoxRuntimeError & error) {
    os << "LoxRuntimeError: " << error.message;
    return os;
}

double parseNumber(std::string lexeme) {
    return atof(lexeme.c_str());
}

LoxValue evaluate(VariableExpression* expr, Environment *env) {
    auto value = env->getVariable(*(expr->variable));
    if (!value.has_value()) {
        std::stringstream ss;
        ss << "Variable " << expr->variable->lexeme << " is not defined";
        runtimeError(ss.str());
    }
    return value.value();
}

LoxValue evaluate(LiteralExpression* expr, Environment *env) {
    switch(expr->literal->type) {
        case +TokenType::NUMBER:
            return { .type=LoxValueType::NUMBER, .number=parseNumber(expr->literal->lexeme)};
        case +TokenType::STRING:
            return { .type=LoxValueType::STRING, .str=&(expr->literal->lexeme)};
        case +TokenType::TRUE:
            return { .type=LoxValueType::BOOLEAN, .boolean=true };
        case +TokenType::FALSE:
            return { .type=LoxValueType::BOOLEAN, .boolean=false };
        case +TokenType::NIL:
            return { .type=LoxValueType::NIL };
        default:
            runtimeError("Invalid literal type");
            break;
    }
}

LoxValue evaluate(GroupingExpression* expr, Environment *environment) {
    return evaluate(expr->grouped, environment);
}

bool isTruthy(LoxValue value) {
    if (value.type == +LoxValueType::NIL) return false;
    if (value.type == +LoxValueType::BOOLEAN) return value.boolean;
    return true;
}

LoxValue evaluate(UnaryExpression* expr, Environment *environment) {
    LoxValue value = evaluate(expr->expression, environment);
    switch (expr->unary->type) {
        case TokenType::MINUS:
            if (value.type == +LoxValueType::NUMBER) {
                value.number = -value.number;
                return value;
            }
            break;
        case TokenType::BANG:
            return {LoxValueType::BOOLEAN, .boolean=!isTruthy(value)};
    }

    runtimeError("Invalid unary operation for value type ");
}

bool isDualType(LoxValue a, LoxValue b, LoxValueType type) {
    return (a.type == b.type) && a.type == type;
}

bool isEqual(LoxValue a, LoxValue b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case LoxValueType::NIL:
            return true;
        case LoxValueType::BOOLEAN:
            return a.boolean == b.boolean;
        case LoxValueType::NUMBER:
            return a.number == b.number;
        case LoxValueType::STRING:
            return *(a.str) == *(b.str);
    }
}

LoxValue evaluate(BinaryExpression* expr, Environment *environment) {
    LoxValue left = evaluate(expr->left, environment);
    LoxValue right = evaluate(expr->right, environment);
    switch (expr->op->type) {
        case TokenType::MINUS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number - right.number};
            }
        break;
        case TokenType::PLUS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number + right.number};
            }
            if (isDualType(left, right, LoxValueType::STRING)) {
                string* concat = new string(*(left.str));
                concat->append(*(right.str));
                return { .type=LoxValueType::STRING, .str=concat};
            }
        break;
        case TokenType::SLASH:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number / right.number};
            }
        break;
        case TokenType::STAR:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::NUMBER, .number=left.number * right.number};
            }
        break;
        case TokenType::LESS:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number < right.number};
            }
        break;
        case TokenType::LESS_EQUAL:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number <= right.number};
            }
        break;
        case TokenType::GREATER:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number > right.number};
            }
        break;
        case TokenType::GREATER_EQUAL:
            if (isDualType(left, right, LoxValueType::NUMBER)) {
                return { .type=LoxValueType::BOOLEAN, .boolean=left.number >= right.number};
            }
        break;
        case TokenType::EQUAL_EQUAL:
            return { .type=LoxValueType::BOOLEAN, .boolean=isEqual(left, right)};
        break;
        case TokenType::BANG_EQUAL:
            return { .type=LoxValueType::BOOLEAN, .boolean=!isEqual(left, right)};
        break;

    }

    runtimeError("Invalid binary operation for value type");
}

LoxValue evaluate(AssignExpression* expr, Environment *env) {
    LoxValue value = evaluate(expr->value, env);
    if(!env->setVariable(*(expr->variable), value)) {
        stringstream ss;
        ss << "Variable " << (expr->variable->lexeme) << " is undefined";
        runtimeError(ss.str());
    }
    return value;
}

LoxValue evaluate(Expression* expr, Environment *environment) {
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
            return evaluate(expr->variableexpression, environment);
        case ExpressionType::AssignExpression:
            return evaluate(expr->assignexpression, environment);
    }
    runtimeError("Unknown expression type");
}

void evaluate(BlockStatement *block, Environment *environment) {
    Environment child(environment);
    for (auto statement : *(block->block)) {
        evaluate(statement, &child);
    }
}

void evaluate(IfStatement *ifStatement, Environment *environment) {
    bool result = isTruthy(evaluate(ifStatement->condition, environment));
    if (result) {
        evaluate(ifStatement->trueBlock, environment);
    } else {
        evaluate(ifStatement->falseBlock, environment);
    }
}

void evaluate(Statement* statement, Environment *environment) {
    switch (statement->type) {
        case StatementType::ExpressionStatement:
            evaluate(statement->expressionstatement->expr, environment);
        break;
        case StatementType::PrintStatement:
            cout << evaluate(statement->printstatement->expr, environment) << endl;
        break;
        case StatementType::BlockStatement:
            evaluate(statement->blockstatement, environment);
        break;
        case StatementType::IfStatement:
            evaluate(statement->ifstatement, environment);
        break;
    }
}

void evaluate(VarDeclaration* varDeclaration, Environment *environment) {
    LoxValue evaluated = evaluate(varDeclaration->expr, environment);
    auto defineResult = environment->defineVariable(*(varDeclaration->identifier));
    if (!defineResult) {
        std::stringstream error;
        error << "Variable " << varDeclaration->identifier->lexeme << " is already defined";
        runtimeError(error.str());
    }
    environment->setVariable(*(varDeclaration->identifier), evaluated);
}

void evaluate(StatementDeclaration* statementDeclaration, Environment *environment) {
    evaluate(statementDeclaration->statement, environment);
}

void evaluate(Declaration* declaration, Environment *environment) {
    switch (declaration->type) {
        case DeclarationType::VarDeclaration:
            evaluate(declaration->vardeclaration, environment);
        break;
        case DeclarationType::StatementDeclaration:
            evaluate(declaration->statementdeclaration, environment);
        break;
    }
}

void CPPLox::Interpreter::run() {
    for (Declaration* declaration : this->program.declarations) {
        evaluate(declaration, &(this->environment));
    }
}


