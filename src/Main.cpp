#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "FileUtils.h"
#include "cpplox/CPPLox.h"
#include "cpplox/TokenScanner.h"
#include "cpplox/Expr.h"
#include "cpplox/ExprPrint.h"

using namespace std;

enum LoxInterpreterResponse {
    SUCCESS = 0,
    FAILURE = 1
};

enum class RunLineResponse {
    SUCCESS,
    FAILURE, 
    QUIT
};

int run(string source) {
    cout << source << endl;
    
    CPPLox::TokenScanner tokenScanner(source);
    vector<Token> tokens = tokenScanner.scanTokens();
    for (auto token : tokens) {
        cout << token << endl;
    }

    cout << print_expression(
        Expression::asBinary(
            Expression::asBinary(
                Expression::asLiteral(
                    LiteralType::STRING,
                    new Token({ "hello", TokenType::STRING, 0 })
                ),
                new Token({ "-", TokenType::PLUS, 0 }),
                Expression::asLiteral(
                    LiteralType::STRING,
                    new Token({ "whassup", TokenType::STRING, 0 })
                )
            ),
            new Token({ "+", TokenType::PLUS, 0 }),
            Expression::asLiteral(
                LiteralType::STRING,
                new Token({ "bye", TokenType::STRING, 0 })
            )
        )
    ) << endl;

    return SUCCESS;
}

int runPrompt() {
    while (true) {
        string line;
        cout << "\n" << "> ";
        cin >> line;
        if (line == "quit" || line == "exit") {
            break;
        }
        run(line);
    }

    return SUCCESS;
}


int runFile(string filename) {
    string source = FileUtils::readFile(filename);
    run(source);
    return LoxInterpreterResponse::SUCCESS;
}

int main(int argc, char** argv) {
    if (argc > 2) {
        cerr << "Usage: lox [script]" << endl;
    } else if (argc == 2) {
        string filename = { argv[1] };
        return runFile(filename);
    } else {
        return runPrompt();
    }

    return LoxInterpreterResponse::FAILURE;
}



