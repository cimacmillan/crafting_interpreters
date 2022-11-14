#pragma once

#include "ast/ast.h"
#include "Interpreter.h"

#include <unordered_map>
#include <string>
#include <stack>

class Analyzer {
private:
    CPPLox::Interpreter *interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;

public:
    Analyzer(CPPLox::Interpreter *interpreter): interpreter(interpreter) {}
		void visit(BinaryExpression *entry);
	void visit(GroupingExpression *entry);
	void visit(UnaryExpression *entry);
	void visit(LiteralExpression *entry);
	void visit(VariableExpression *entry);
	void visit(LogicalExpression *entry);
	void visit(AssignExpression *entry);
	void visit(CallExpression *entry);
	void visit(Expression *entry) {
		switch(entry->type) {
			case ExpressionType::BinaryExpression:
				this->visit(entry->binaryexpression);
				break;
			case ExpressionType::GroupingExpression:
				this->visit(entry->groupingexpression);
				break;
			case ExpressionType::UnaryExpression:
				this->visit(entry->unaryexpression);
				break;
			case ExpressionType::LiteralExpression:
				this->visit(entry->literalexpression);
				break;
			case ExpressionType::VariableExpression:
				this->visit(entry->variableexpression);
				break;
			case ExpressionType::AssignExpression:
				this->visit(entry->assignexpression);
				break;
			case ExpressionType::LogicalExpression:
				this->visit(entry->logicalexpression);
				break;
			case ExpressionType::CallExpression:
				this->visit(entry->callexpression);
				break;
		}
	}
	void visit(ExpressionStatement *entry);
	void visit(PrintStatement *entry);
	void visit(BlockStatement *entry);
	void visit(IfStatement *entry);
	void visit(WhileStatement *entry);
	void visit(ReturnStatement *entry);
	void visit(Statement *entry) {
		switch(entry->type) {
			case StatementType::ExpressionStatement:
				this->visit(entry->expressionstatement);
				break;
			case StatementType::PrintStatement:
				this->visit(entry->printstatement);
				break;
			case StatementType::BlockStatement:
				this->visit(entry->blockstatement);
				break;
			case StatementType::IfStatement:
				this->visit(entry->ifstatement);
				break;
			case StatementType::WhileStatement:
				this->visit(entry->whilestatement);
				break;
			case StatementType::ReturnStatement:
				this->visit(entry->returnstatement);
				break;
		}
	}
	void visit(VarDeclaration *entry);
	void visit(StatementDeclaration *entry);
	void visit(FunctionDeclaration *entry);
	void visit(Declaration *entry) {
		switch(entry->type) {
			case DeclarationType::VarDeclaration:
				this->visit(entry->vardeclaration);
				break;
			case DeclarationType::StatementDeclaration:
				this->visit(entry->statementdeclaration);
				break;
			case DeclarationType::FunctionDeclaration:
				this->visit(entry->functiondeclaration);
				break;
		}
	}
	void visit(LoxProgram *entry);


	void declare(std::string lexeme);
	void resolve(VariableExpression* expr);
};
