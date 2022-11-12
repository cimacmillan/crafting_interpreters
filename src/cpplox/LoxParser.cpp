
#include <vector>
#include <string>
#include <sstream>
#include "Token.h"
#include "Expr.h"
#include "LoxParser.h"
#include "CPPLox.h"

using namespace std;

void CPPLox::LoxParser::consume(TokenType type) {
    if (!this->match(type)) {
        stringstream ss;
        ss << "Expected token ";
        ss << type;
        CPPLox::fatal_token(this->peek(), ss.str());
    }
}

bool CPPLox::LoxParser::match(TokenType type) {
    if (this->check(type)) {
        this->advance();
        return true;
    }
    return false;
}

bool CPPLox::LoxParser::check(TokenType type) {
    return this->peek().type == type;
}

Token CPPLox::LoxParser::advance() {
    Token eaten = this->peek();
    if (this->isAtEnd()) return eaten;
    this->current++;
    return eaten;
}

bool CPPLox::LoxParser::isAtEnd() {
    switch (this->peek().type) {
        case TokenType::FILE_END:
            return true;
        default: 
            return false;
    }
}

Token CPPLox::LoxParser::peek() {
    return this->tokens[this->current];
}

Token CPPLox::LoxParser::previous() {
    return this->tokens[this->current - 1];
}

Expression* CPPLox::LoxParser::primary() {
    if (this->match(TokenType::NUMBER)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::LiteralExpression,
            .literalexpression = new LiteralExpression({ previous })
        });
    }
    if (this->match(TokenType::STRING)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::LiteralExpression,
            .literalexpression = new LiteralExpression({ previous })
        });
    }
    if (this->match(TokenType::TRUE)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::LiteralExpression,
            .literalexpression = new LiteralExpression({ previous })
        });
    }
    if (this->match(TokenType::FALSE)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::LiteralExpression,
            .literalexpression = new LiteralExpression({ previous })
        });
    }
    if (this->match(TokenType::NIL)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::LiteralExpression,
            .literalexpression = new LiteralExpression({ previous })
        });
    }
    if (this->match(TokenType::IDENTIFIER)) {
        Token* previous = new Token(this->previous());
        return new Expression({
            .type = +ExpressionType::VariableExpression,
            .variableexpression = new VariableExpression({ previous })
        });
    }

    if (this->match(TokenType::LEFT_PAREN)) {
        Token* left = new Token(this->previous());
        Expression* expr = this->expression();
        if (this->match(TokenType::RIGHT_PAREN)) {
            Token* right = new Token(this->previous());
            return new Expression({
                .type = +ExpressionType::GroupingExpression,
                .groupingexpression = new GroupingExpression({ left, expr, right })
            });
        }
        CPPLox::fatal_token(this->peek(), "expected right parenthesis");
    }

    CPPLox::fatal_token(this->peek(), "Expected expression");
    return nullptr;
}

Expression* CPPLox::LoxParser::call() {
    Expression* primary = this->primary();
    if (this->match(TokenType::LEFT_PAREN)) {
        auto args = new vector<Expression*>();
        if (!this->check(TokenType::RIGHT_PAREN)) {
            do {
                args->push_back(this->expression());
            } while (this->match(TokenType::COMMA));
        }
        this->consume(TokenType::RIGHT_PAREN);
        return Expression::asCallExpression(new CallExpression({primary, args}));
    }
    return primary;
}

Expression* CPPLox::LoxParser::unary() {
    if (this->match(TokenType::BANG) || this->match(TokenType::MINUS)) {
        Token *previous = new Token(this->previous());
        Expression* again = this->unary();
        return new Expression({
            .type = +ExpressionType::UnaryExpression,
            .unaryexpression = new UnaryExpression({ again, previous })
        });
    }
    return this->call();
}

Expression* CPPLox::LoxParser::factor() {
    Expression* left = this->unary();
    while (this->match(TokenType::SLASH) || this->match(TokenType::STAR)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->unary();
        left = new Expression({
            .type = +ExpressionType::BinaryExpression,
            .binaryexpression = new BinaryExpression({ left, previous, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::term() {
    Expression* left = this->factor();
    while (this->match(TokenType::MINUS) || this->match(TokenType::PLUS)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->factor();
        left = new Expression({
            .type = +ExpressionType::BinaryExpression,
            .binaryexpression = new BinaryExpression({ left, previous, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::comparison() {
    Expression* left = this->term();
    while (this->match(TokenType::GREATER) || this->match(TokenType::GREATER_EQUAL) || this->match(TokenType::LESS) || this->match(TokenType::LESS_EQUAL)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->term();
        left = new Expression({
            .type = +ExpressionType::BinaryExpression,
            .binaryexpression = new BinaryExpression({ left, previous, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::equality() {
    Expression* left = this->comparison();
    while (this->match(TokenType::BANG_EQUAL) || this->match(TokenType::EQUAL_EQUAL)) {
        Token* previous = new Token(this->previous());
        Expression* right = this->comparison();
        left = new Expression({
            .type = +ExpressionType::BinaryExpression,
            .binaryexpression = new BinaryExpression({ left, previous, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::logicalAnd() {
    Expression* left = equality();
    if (this->match(TokenType::AND)) {
        Token* op = new Token(this->previous());
        Expression* right = equality();
        left = new Expression({
            .type = +ExpressionType::LogicalExpression,
            .logicalexpression = new LogicalExpression({ left, op, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::logicalOr() {
    Expression* left = logicalAnd();
    if (this->match(TokenType::OR)) {
        Token* op = new Token(this->previous());
        Expression* right = logicalAnd();
        left = new Expression({
            .type = +ExpressionType::LogicalExpression,
            .logicalexpression = new LogicalExpression({ left, op, right })
        });
    }
    return left;
}

Expression* CPPLox::LoxParser::assignment() {
    Expression* lvalue = logicalOr();
    if (this->match(TokenType::EQUAL)) {
        if (lvalue->type != +ExpressionType::VariableExpression) {
            CPPLox::fatal_token(this->peek(), "Expected variable for left of assignemnt =");
        }
        Expression* rvalue = assignment();
        return new Expression({
            .type = +ExpressionType::AssignExpression,
            .assignexpression = new AssignExpression({ lvalue->variableexpression->variable, rvalue })
        });
    }
    return lvalue;
}

Expression* CPPLox::LoxParser::expression() {
    return this->assignment();
}

Statement* CPPLox::LoxParser::printExpression() {
    Token* print = new Token(this->previous());
    Expression* expr = this->expression();
    if (!this->match(TokenType::SEMICOLON)) {
        CPPLox::fatal_token(this->peek(), "Expected semicolon");
    }
    Token* semi = new Token(this->previous());
    return new Statement({
        .type = +StatementType::PrintStatement,
        .printstatement = new PrintStatement({print, expr, semi})
    });
}

Statement* CPPLox::LoxParser::statementExpression() {
    Expression* expr = this->expression();
    if (!this->match(TokenType::SEMICOLON)) {
        CPPLox::fatal_token(this->peek(), "Expected semicolon");
    }
    Token* semi = new Token(this->previous());
    return new Statement({
        .type = +StatementType::ExpressionStatement,
        .expressionstatement = new ExpressionStatement({expr, semi})
    });
}

Statement* CPPLox::LoxParser::blockStatement() {
    vector<struct Declaration*> *declarations = new vector<Declaration*>;

    while(!this->check(TokenType::RIGHT_BRACE) && !this->isAtEnd()) {
        declarations->push_back(this->declaration());
    }

    if (!this->match(TokenType::RIGHT_BRACE)) {
        CPPLox::fatal_token(this->peek(), "Expected closing brace");
    }

    return new Statement({
        .type = +StatementType::BlockStatement,
        .blockstatement = new BlockStatement({ declarations })
    });
}

Statement* CPPLox::LoxParser::ifStatement() {
    this->consume(TokenType::LEFT_PAREN);
    Expression *condition = this->expression();
    this->consume(TokenType::RIGHT_PAREN);
    Statement *trueBlock = this->statement();
    Statement *falseBlock = nullptr;
    if (this->match(TokenType::ELSE)) {
        falseBlock = this->statement();
    }
    return new Statement({
        .type = +StatementType::IfStatement,
        .ifstatement = new IfStatement({ condition, trueBlock, falseBlock })
    });
}

Statement* CPPLox::LoxParser::whileStatement() {
    this->consume(TokenType::LEFT_PAREN);
    Expression *condition = this->expression();
    this->consume(TokenType::RIGHT_PAREN);
    Statement *block = this->statement();
    return new Statement({
        .type = +StatementType::WhileStatement,
        .whilestatement = new WhileStatement({ condition, block })
    });
}

Statement* CPPLox::LoxParser::forStatement() {
    this->consume(TokenType::LEFT_PAREN);
    auto block = new vector<Declaration*>();

    if (this->match(TokenType::VAR)) {
        block->push_back(this->var_declaration());
    } else if (!this->match(TokenType::SEMICOLON)) {
        block->push_back(new Declaration({
            .type = DeclarationType::StatementDeclaration,
            .statementdeclaration = new StatementDeclaration({
                this->statementExpression()
            })
        }));
    }

    Statement* condition = nullptr;
    if (!this->match(TokenType::SEMICOLON)) {
        condition = this->statementExpression();
    }

    Expression* increment = nullptr;
    if (!this->match(TokenType::RIGHT_PAREN)) {
        increment = this->expression();
        this->consume(TokenType::RIGHT_PAREN);
    }

    auto forBlock = this->statement();

    if (condition == nullptr) {
        condition = new Statement({
            .type = +StatementType::ExpressionStatement,
            .expressionstatement = new ExpressionStatement({
                .expr = new Expression({
                    .type = +ExpressionType::LiteralExpression,
                    .literalexpression = new LiteralExpression({
                        new Token({
                            .lexeme =  "true",
                            .type = +TokenType::TRUE,
                            .line = 0
                        })
                    })
                })
            })
        });
    }

    auto whileBlock = new vector<Declaration*>({
        new Declaration({
            .type = +DeclarationType::StatementDeclaration,
            .statementdeclaration = new StatementDeclaration({ forBlock })
        })
    });

    if (increment) {
        whileBlock->push_back(new Declaration({
            .type = +DeclarationType::StatementDeclaration,
            .statementdeclaration = new StatementDeclaration({
                new Statement({
                    .type = +StatementType::ExpressionStatement,
                    .expressionstatement = new ExpressionStatement({
                        increment
                    })
                })
            })
        }));
    }

    auto whileStatement = new Statement({
        .type = +StatementType::WhileStatement,
        .whilestatement = new WhileStatement({ 
            condition->expressionstatement->expr, 
            new Statement({
                .type = +StatementType::BlockStatement,
                .blockstatement = new BlockStatement({ whileBlock })
            })
        })
    });

    block->push_back(new Declaration({
        .type = +DeclarationType::StatementDeclaration,
        .statementdeclaration = new StatementDeclaration({ whileStatement })
    }));

    return new Statement({
        .type = +StatementType::BlockStatement,
        .blockstatement = new BlockStatement({ block })
    });

}

Statement* CPPLox::LoxParser::statement() {
    if (this->match(TokenType::PRINT)) {
        return this->printExpression();
    }
    if (this->match(TokenType::LEFT_BRACE)) {
        return this->blockStatement();
    }
    if (this->match(TokenType::IF)) {
        return this->ifStatement();
    }
    if (this->match(TokenType::WHILE)) {
        return this->whileStatement();
    }
    if (this->match(TokenType::FOR)) {
        return this->forStatement();
    }
    return this->statementExpression();
}

Declaration* CPPLox::LoxParser::var_declaration() {
    Token* var = new Token(this->previous());
    
    if (!this->match(TokenType::IDENTIFIER)) {
        CPPLox::fatal_token(this->peek(), "Expected identifier");
    }

    Token* identifier = new Token(this->previous());

    if (this->match(TokenType::EQUAL)) {
        Token* equal = new Token(this->previous());
        Expression* expr = this->expression();
        if (!this->match(TokenType::SEMICOLON)) {
            CPPLox::fatal_token(this->peek(), "Expected semicolon");
        }
        Token* semi = new Token(this->previous());
        return new Declaration({
            .type = +DeclarationType::VarDeclaration,
            .vardeclaration = new VarDeclaration({
                var, identifier, equal, expr, semi
            })
        });
    } else {
        if (!this->match(TokenType::SEMICOLON)) {
            CPPLox::fatal_token(this->peek(), "Expected semicolon");
        }
        Token* semi = new Token(this->previous());
         return new Declaration({
            .type = +DeclarationType::VarDeclaration,
            .vardeclaration = new VarDeclaration({
                var, identifier, nullptr, nullptr, semi
            })
        });
    }
}

Declaration* CPPLox::LoxParser::statement_declaration() {
    return new Declaration({
        .type = +DeclarationType::StatementDeclaration,
        .statementdeclaration = new StatementDeclaration({this->statement()})
    });
}

Declaration* CPPLox::LoxParser::declaration() {
    if (match(TokenType::VAR)) {
        return this->var_declaration();
    }
    return this->statement_declaration();
}

LoxProgram CPPLox::LoxParser::parse() {
    LoxProgram program;
    while (!this->isAtEnd()) {
        program.declarations.push_back(this->declaration());
    }
    return program;
}
