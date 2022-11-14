#include "Analyzer.h"
#include "CPPLox.h"

void Analyzer::declare(Token token) {
    if (this->scopes.back().find(token.lexeme) != this->scopes.back().end()) {
        CPPLox::fatal_token(token, "cannot redefine variable");
    }
    this->scopes.back()[token.lexeme] = true;
}

void Analyzer::resolve(VariableExpression *expr) {
    int hops = 0;
    int size = this->scopes.size();
    for (int i = 0; i < size; i++) {
        bool result = this->scopes[size - i - 1][expr->variable->lexeme];
        if (result) {
            this->interpreter->setVariableHops(expr, i);
            return;
        }
    }
    CPPLox::fatal_token(*(expr->variable), "is used before it is defined");
}

void Analyzer::visit(BinaryExpression *entry) {
    this->visit(entry->left);
    this->visit(entry->right);
}
void Analyzer::visit(UnaryExpression *entry) {
    this->visit(entry->expression);
}
void Analyzer::visit(GroupingExpression *entry) {
    this->visit(entry->grouped);
}
void Analyzer::visit(LiteralExpression *entry) {
    // no-op
}
void Analyzer::visit(VariableExpression *entry) {
    this->resolve(entry);
}
void Analyzer::visit(LogicalExpression *entry) {
    this->visit(entry->left);
    this->visit(entry->right);
}
void Analyzer::visit(AssignExpression *entry) {
    this->visit(entry->value);
}
void Analyzer::visit(CallExpression *entry) {
    this->visit(entry->callee);
    for (auto arg : *(entry->arguments)) {
        this->visit(arg);
    }
}
void Analyzer::visit(ExpressionStatement *entry) {
    this->visit(entry->expr);
}
void Analyzer::visit(PrintStatement *entry) {
    this->visit(entry->expr);
}
void Analyzer::visit(BlockStatement *entry, std::unordered_map<string, bool> args) {
    this->scopes.push_back(args);
    auto blocks = *(entry->block);
    for (auto block : blocks) {
        this->visit(block);
    }
    this->scopes.pop_back();
}
void Analyzer::visit(IfStatement *entry) {
    this->visit(entry->condition);
    this->visit(entry->trueBlock);
    if (entry->falseBlock) {
        this->visit(entry->falseBlock);
    }
}
void Analyzer::visit(WhileStatement *entry) {
    this->visit(entry->condition);
    this->visit(entry->block);
}
void Analyzer::visit(ReturnStatement *entry) {
    if (this->funcType == FunctionType::NONE) {
        CPPLox::fatal(0, "Cannot return outside of function");
    }
    if (entry->expr) {
        this->visit(entry->expr);
    }
}
void Analyzer::visit(VarDeclaration *entry) {
    std::string name = entry->identifier->lexeme;
    if (entry->expr) {
        this->visit(entry->expr);
    }
    this->declare(*(entry->identifier));
}
void Analyzer::visit(StatementDeclaration *entry) {
    this->visit(entry->statement);
}
void Analyzer::visit(FunctionDeclaration *entry) {
    std::string name = entry->identifier->lexeme;
    this->declare(*(entry->identifier));
    auto scope = std::unordered_map<string, bool>();
    for (auto token : *(entry->argIdentifiers)) {
        scope[token->lexeme] = true;
    }
    auto temp = this->funcType;
    this->funcType = FunctionType::FUNCTION;
    this->visit(entry->block->blockstatement, scope);
    this->funcType = temp;
}
void Analyzer::visit(LoxProgram *entry) {
    auto global = std::unordered_map<std::string, bool>();
    this->scopes.push_back(global);
    for (auto decl : *(entry->program)) {
        this->visit(decl);
    }
}
