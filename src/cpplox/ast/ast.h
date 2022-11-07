// This is generated by scripts/ASTGenerator.ts
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
struct AssignExpression {
	struct Token *variable;
	struct Expression *value;
};
BETTER_ENUM(ExpressionType, char, BinaryExpression,GroupingExpression,UnaryExpression,LiteralExpression,VariableExpression,AssignExpression);
struct Expression {
	ExpressionType type;
	union {
		BinaryExpression *binaryexpression;
		GroupingExpression *groupingexpression;
		UnaryExpression *unaryexpression;
		LiteralExpression *literalexpression;
		VariableExpression *variableexpression;
		AssignExpression *assignexpression;
	};
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
BETTER_ENUM(StatementType, char, ExpressionStatement,PrintStatement,BlockStatement);
struct Statement {
	StatementType type;
	union {
		ExpressionStatement *expressionstatement;
		PrintStatement *printstatement;
		BlockStatement *blockstatement;
	};
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
BETTER_ENUM(DeclarationType, char, VarDeclaration,StatementDeclaration);
struct Declaration {
	DeclarationType type;
	union {
		VarDeclaration *vardeclaration;
		StatementDeclaration *statementdeclaration;
	};
};
