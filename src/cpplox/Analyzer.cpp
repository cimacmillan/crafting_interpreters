#include "Analyzer.h"

void Analyzer::visit(BinaryExpression *entry) {

}
void Analyzer::visit(UnaryExpression *entry) {

}
void Analyzer::visit(GroupingExpression *entry) {

}
void Analyzer::visit(LiteralExpression *entry) {

}
void Analyzer::visit(VariableExpression *entry) {

}
void Analyzer::visit(LogicalExpression *entry) {

}
void Analyzer::visit(AssignExpression *entry) {

}
void Analyzer::visit(CallExpression *entry) {

}
void Analyzer::visit(ExpressionStatement *entry) {

}
void Analyzer::visit(PrintStatement *entry) {

}
void Analyzer::visit(BlockStatement *entry) {

}
void Analyzer::visit(IfStatement *entry) {

}
void Analyzer::visit(WhileStatement *entry) {

}
void Analyzer::visit(ReturnStatement *entry) {

}
void Analyzer::visit(VarDeclaration *entry) {

}
void Analyzer::visit(StatementDeclaration *entry) {

}
void Analyzer::visit(FunctionDeclaration *entry) {

}
void Analyzer::visit(LoxProgram *entry) {
    for (auto decl : *(entry->program)) {
        this->visit(decl);
    }
}

