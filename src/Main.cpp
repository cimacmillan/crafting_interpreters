#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>


#include "FileUtils.h"
#include "cpplox/CPPLox.h"
#include "cpplox/TokenScanner.h"
#include "cpplox/Expr.h"
#include "cpplox/LoxParser.h"
#include "cpplox/Interpreter.h"
#include "cpplox/LoxFunction.h"
#include "cpplox/Analyzer.h"


using namespace std;
using namespace std::chrono;

typedef std::numeric_limits< double > dbl;

enum LoxInterpreterResponse {
    SUCCESS = 0,
    FAILURE = 1
};

enum class RunLineResponse {
    SUCCESS,
    FAILURE, 
    QUIT
};

int run(string source, Environment env) {
    CPPLox::TokenScanner tokenScanner(source);
    vector<Token> tokens = tokenScanner.scanTokens();
    CPPLox::LoxParser parser(tokens);
    LoxProgram program = parser.parse();
    CPPLox::Interpreter interpreter(program, env);
    Analyzer analyzer(&interpreter);
    analyzer.visit(&program);
    try {

        interpreter.run();
    } catch (LoxRuntimeError error) {
        cout << error << endl;
    }

    return SUCCESS;
}

int runPrompt(Environment env) {
    while (true) {
        string line;
        cout << "\n" << "> ";
        getline(cin, line);
        if (line == "quit" || line == "exit") {
            break;
        }
        run(line, env);
    }

    return SUCCESS;
}


int runFile(string filename, Environment env) {
    string source = FileUtils::readFile(filename);
    run(source, env);
    return LoxInterpreterResponse::SUCCESS;
}

LoxValue clock(vector<LoxValue> args) {
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    
    return (LoxValue){.type=LoxValueType::NUMBER, .number=(double)ms.count()};
}

LoxValue prints(vector<LoxValue> args) {
    for (auto arg : args) {
        cout << arg;
    }
    cout << endl;
    return (LoxValue){.type=LoxValueType::NIL};

}

Environment createDefaultEnvironment() {
    Environment env(nullptr, {});
    env.defineVariable("clock");
    env.setVariable("clock", (LoxValue){
        .type= +LoxValueType::CALLABLE,
        .callable=new LoxNativeFunction(clock)
    });
    env.defineVariable("prints");
    env.setVariable("prints", (LoxValue) {
        .type= +LoxValueType::CALLABLE,
        .callable=new LoxNativeFunction(prints)
    });
    return env;
}

int main(int argc, char** argv) {
    Environment env = createDefaultEnvironment();
    cout.precision(dbl::max_digits10);

    if (argc > 2) {
        cerr << "Usage: lox [script]" << endl;
    } else if (argc == 2) {
        string filename = { argv[1] };
        return runFile(filename, env);
    } else {
        return runPrompt(env);
    }

    return LoxInterpreterResponse::FAILURE;
}



