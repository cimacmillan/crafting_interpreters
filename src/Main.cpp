#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "FileUtils.h"
#include "cpplox/CPPLox.h"

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
    
    vector<Token> tokens = CPPLox::scanTokens(source);
    for (auto token : tokens) {
        cout << token << endl;
    }

    CPPLox::report(1, "Hello");
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



