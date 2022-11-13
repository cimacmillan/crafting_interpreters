// This is generated by scripts/ASTGenerator.ts
#pragma once
#include <enum.h>
#include <vector>
using namespace std;

struct BinaryExpression {
	struct Expression *left;
	struct Token *op;
	struct Expression *right;
};
struct GroupingExpression {
	struct Token *open;
	struct Expression *grouped;
	struct Token *close;
};
struct UnaryExpression {
	struct Expression *expression;
	struct Token *unary;
};
struct LiteralExpression {
	struct Token *literal;
};
struct VariableExpression {
	struct Token *variable;
};
struct LogicalExpression {
	struct Expression *left;
	struct Token *op;
	struct Expression *right;
};
struct AssignExpression {
	struct Token *variable;
	struct Expression *value;
};
struct CallExpression {
	struct Expression *callee;
	struct vector<Expression*> *arguments;
};
BETTER_ENUM(ExpressionType, char, BinaryExpression,GroupingExpression,UnaryExpression,LiteralExpression,VariableExpression,AssignExpression,LogicalExpression,CallExpression);
struct Expression {
	ExpressionType type;
	union {
		BinaryExpression *binaryexpression;
		GroupingExpression *groupingexpression;
		UnaryExpression *unaryexpression;
		LiteralExpression *literalexpression;
		VariableExpression *variableexpression;
		AssignExpression *assignexpression;
		LogicalExpression *logicalexpression;
		CallExpression *callexpression;
	};
	static Expression* asBinaryExpression(BinaryExpression *binaryexpression) {
		return new Expression({.type=ExpressionType::BinaryExpression, .binaryexpression=binaryexpression});
	}
	static Expression* asGroupingExpression(GroupingExpression *groupingexpression) {
		return new Expression({.type=ExpressionType::GroupingExpression, .groupingexpression=groupingexpression});
	}
	static Expression* asUnaryExpression(UnaryExpression *unaryexpression) {
		return new Expression({.type=ExpressionType::UnaryExpression, .unaryexpression=unaryexpression});
	}
	static Expression* asLiteralExpression(LiteralExpression *literalexpression) {
		return new Expression({.type=ExpressionType::LiteralExpression, .literalexpression=literalexpression});
	}
	static Expression* asVariableExpression(VariableExpression *variableexpression) {
		return new Expression({.type=ExpressionType::VariableExpression, .variableexpression=variableexpression});
	}
	static Expression* asAssignExpression(AssignExpression *assignexpression) {
		return new Expression({.type=ExpressionType::AssignExpression, .assignexpression=assignexpression});
	}
	static Expression* asLogicalExpression(LogicalExpression *logicalexpression) {
		return new Expression({.type=ExpressionType::LogicalExpression, .logicalexpression=logicalexpression});
	}
	static Expression* asCallExpression(CallExpression *callexpression) {
		return new Expression({.type=ExpressionType::CallExpression, .callexpression=callexpression});
	}
};
struct ExpressionStatement {
	struct Expression *expr;
	struct Token *semicolon;
};
struct PrintStatement {
	struct Token *print;
	struct Expression *expr;
	struct Token *semicolon;
};
struct BlockStatement {
	struct vector<struct Declaration*> *block;
};
struct IfStatement {
	struct Expression *condition;
	struct Statement *trueBlock;
	struct Statement *falseBlock;
};
struct WhileStatement {
	struct Expression *condition;
	struct Statement *block;
};
struct ReturnStatement {
	struct Expression *expr;
};
BETTER_ENUM(StatementType, char, ExpressionStatement,PrintStatement,BlockStatement,IfStatement,WhileStatement,ReturnStatement);
struct Statement {
	StatementType type;
	union {
		ExpressionStatement *expressionstatement;
		PrintStatement *printstatement;
		BlockStatement *blockstatement;
		IfStatement *ifstatement;
		WhileStatement *whilestatement;
		ReturnStatement *returnstatement;
	};
	static Statement* asExpressionStatement(ExpressionStatement *expressionstatement) {
		return new Statement({.type=StatementType::ExpressionStatement, .expressionstatement=expressionstatement});
	}
	static Statement* asPrintStatement(PrintStatement *printstatement) {
		return new Statement({.type=StatementType::PrintStatement, .printstatement=printstatement});
	}
	static Statement* asBlockStatement(BlockStatement *blockstatement) {
		return new Statement({.type=StatementType::BlockStatement, .blockstatement=blockstatement});
	}
	static Statement* asIfStatement(IfStatement *ifstatement) {
		return new Statement({.type=StatementType::IfStatement, .ifstatement=ifstatement});
	}
	static Statement* asWhileStatement(WhileStatement *whilestatement) {
		return new Statement({.type=StatementType::WhileStatement, .whilestatement=whilestatement});
	}
	static Statement* asReturnStatement(ReturnStatement *returnstatement) {
		return new Statement({.type=StatementType::ReturnStatement, .returnstatement=returnstatement});
	}
};
struct VarDeclaration {
	struct Token *var;
	struct Token *identifier;
	struct Token *equals;
	struct Expression *expr;
	struct Token *semi;
};
struct StatementDeclaration {
	struct Statement *statement;
	struct Token *identifier;
	struct Token *equals;
	struct Expression *expr;
	struct Token *semi;
};
struct FunctionDeclaration {
	struct Token *identifier;
	struct vector<Token*> *argIdentifiers;
	struct Statement *block;
};
BETTER_ENUM(DeclarationType, char, VarDeclaration,StatementDeclaration,FunctionDeclaration);
struct Declaration {
	DeclarationType type;
	union {
		VarDeclaration *vardeclaration;
		StatementDeclaration *statementdeclaration;
		FunctionDeclaration *functiondeclaration;
	};
	static Declaration* asVarDeclaration(VarDeclaration *vardeclaration) {
		return new Declaration({.type=DeclarationType::VarDeclaration, .vardeclaration=vardeclaration});
	}
	static Declaration* asStatementDeclaration(StatementDeclaration *statementdeclaration) {
		return new Declaration({.type=DeclarationType::StatementDeclaration, .statementdeclaration=statementdeclaration});
	}
	static Declaration* asFunctionDeclaration(FunctionDeclaration *functiondeclaration) {
		return new Declaration({.type=DeclarationType::FunctionDeclaration, .functiondeclaration=functiondeclaration});
	}
};
struct LoxProgram {
	struct vector<Declaration*> *program;
};
class AstVisitor {
public:
	virtual void visit(BinaryExpression *entry) = 0;
	virtual void visit(GroupingExpression *entry) = 0;
	virtual void visit(UnaryExpression *entry) = 0;
	virtual void visit(LiteralExpression *entry) = 0;
	virtual void visit(VariableExpression *entry) = 0;
	virtual void visit(LogicalExpression *entry) = 0;
	virtual void visit(AssignExpression *entry) = 0;
	virtual void visit(CallExpression *entry) = 0;
	void visit(Expression *entry) {
		switch(entry->type) {
			case ExpressionType::BinaryExpression:
				return this->visit(entry);
			case ExpressionType::GroupingExpression:
				return this->visit(entry);
			case ExpressionType::UnaryExpression:
				return this->visit(entry);
			case ExpressionType::LiteralExpression:
				return this->visit(entry);
			case ExpressionType::VariableExpression:
				return this->visit(entry);
			case ExpressionType::AssignExpression:
				return this->visit(entry);
			case ExpressionType::LogicalExpression:
				return this->visit(entry);
			case ExpressionType::CallExpression:
				return this->visit(entry);
		}
	}
	virtual void visit(ExpressionStatement *entry) = 0;
	virtual void visit(PrintStatement *entry) = 0;
	virtual void visit(BlockStatement *entry) = 0;
	virtual void visit(IfStatement *entry) = 0;
	virtual void visit(WhileStatement *entry) = 0;
	virtual void visit(ReturnStatement *entry) = 0;
	void visit(Statement *entry) {
		switch(entry->type) {
			case StatementType::ExpressionStatement:
				return this->visit(entry);
			case StatementType::PrintStatement:
				return this->visit(entry);
			case StatementType::BlockStatement:
				return this->visit(entry);
			case StatementType::IfStatement:
				return this->visit(entry);
			case StatementType::WhileStatement:
				return this->visit(entry);
			case StatementType::ReturnStatement:
				return this->visit(entry);
		}
	}
	virtual void visit(VarDeclaration *entry) = 0;
	virtual void visit(StatementDeclaration *entry) = 0;
	virtual void visit(FunctionDeclaration *entry) = 0;
	void visit(Declaration *entry) {
		switch(entry->type) {
			case DeclarationType::VarDeclaration:
				return this->visit(entry);
			case DeclarationType::StatementDeclaration:
				return this->visit(entry);
			case DeclarationType::FunctionDeclaration:
				return this->visit(entry);
		}
	}
	virtual void visit(LoxProgram *entry) = 0;
};
