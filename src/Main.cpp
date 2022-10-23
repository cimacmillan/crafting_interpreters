#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "FileUtils.h"
#include "cpplox/CPPLox.h"
#include "cpplox/TokenScanner.h"
#include "cpplox/Expr.h"
#include "cpplox/ExprPrint.h"
#include "cpplox/LoxParser.h"
#include "cpplox/Interpreter.h"

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

    CPPLox::LoxParser parser(tokens);
    vector<Statement*> statements = parser.parse();

    try {
        CPPLox::Interpreter interpreter(statements);
        interpreter.run();
    } catch (LoxRuntimeError error) {
        cout << error << endl;
    }

    return SUCCESS;
}

int runPrompt() {
    while (true) {
        string line;
        cout << "\n" << "> ";
        getline(cin, line);
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



