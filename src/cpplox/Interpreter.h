#include "Expr.h"
#include <enum.h>
#include <iostream>

using namespace std;

BETTER_ENUM(LoxValueType, char, NUMBER, STRING, BOOLEAN, NIL);

struct LoxValue {
    LoxValueType type;
    union {
        double number;
        std::string *str;
        bool boolean;
    };

    void print() {
        switch (type) {
            case LoxValueType::NUMBER:
                cout << number << endl;
                break;
            case LoxValueType::STRING:
                cout << "\"" << *str << "\"" << endl;
                break;
            case LoxValueType::BOOLEAN:
                cout << (boolean ? "true" : "false") << endl;
                break;
            case LoxValueType::NIL:
                cout << "nil" << endl;
                break;
        }
    }
};

LoxValue evaluate(LiteralExpression* expr);
LoxValue evaluate(GroupingExpression* expr);
LoxValue evaluate(UnaryExpression* expr);
LoxValue evaluate(BinaryExpression* expr);
LoxValue evaluate(Expression* expr);

namespace CPPLox {
class Interpreter {
private:
    Expression* expr;

public:
    Interpreter(Expression* expr): expr(expr) {}
    void run();

};
};

