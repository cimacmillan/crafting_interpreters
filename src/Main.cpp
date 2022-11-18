#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "FileUtils.h"
#include "cpplox/Analyzer.h"
#include "cpplox/CPPLox.h"
#include "cpplox/Expr.h"
#include "cpplox/Interpreter.h"
#include "cpplox/LoxError.h"
#include "cpplox/LoxFunction.h"
#include "cpplox/LoxParser.h"
#include "cpplox/TokenScanner.h"

using namespace std;
using namespace std::chrono;

typedef std::numeric_limits<double> dbl;

enum LoxInterpreterResponse { SUCCESS = 0, FAILURE = 1 };

enum class RunLineResponse { SUCCESS, FAILURE, QUIT };

int run(string source, Environment env) {
    CPPLox::TokenScanner tokenScanner(source);
    vector<Token> tokens = tokenScanner.scanTokens();
    CPPLox::LoxParser parser(tokens);
    LoxProgram program = parser.parse();
    Interpreter interpreter(program, env);
    Analyzer analyzer(&interpreter);
    analyzer.visit(&program);
    try {

        interpreter.run();
    } catch (LoxRuntimeError error) {
        cerr << error << endl;
        exit(1);
    }

    return SUCCESS;
}

int runPrompt(Environment env) {
    while (true) {
        string line;
        cout << "\n"
             << "> ";
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
    milliseconds ms =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    return (LoxValue){.type = LoxValueType::NUMBER,
                      .number = (double)ms.count()};
}

LoxValue prints(vector<LoxValue> args) {
    for (auto arg : args) {
        cout << arg;
    }
    cout << endl;
    return (LoxValue){.type = LoxValueType::NIL};
}

LoxValue program_exit_code(vector<LoxValue> args) {
    if (args.size() > 2 && args[2].type == +LoxValueType::STRING) {
        auto value = args[2].str;
        cerr << *value << endl;
    }

    if (args.size() > 1 && args[1].type == +LoxValueType::STRING) {
        auto value = args[1].str;
        cout << *value << endl;
    }

    if (args.size() > 0 && args[0].type == +LoxValueType::NUMBER) {
        auto value = args[0].number;
        exit(value);
    }
}

Environment createDefaultEnvironment() {
    Environment env(nullptr, {});
    env.defineVariable("clock");
    env.setVariable("clock", (LoxValue){.type = +LoxValueType::CALLABLE,
                                        .callable = new LoxNativeFunction(
                                            clock, "fun clock()")});
    env.defineVariable("prints");
    env.setVariable("prints", (LoxValue){.type = +LoxValueType::CALLABLE,
                                         .callable = new LoxNativeFunction(
                                             prints, "fun prints(args...)")});
    env.defineVariable("exit");
    env.setVariable("exit", (LoxValue){.type = +LoxValueType::CALLABLE,
                                         .callable = new LoxNativeFunction(
                                             program_exit_code, "fun exit(number, stdout, stderr)")});                                
    return env;
}

int main(int argc, char **argv) {
    Environment env = createDefaultEnvironment();
    cout.precision(dbl::max_digits10);

    if (argc > 2) {
        cerr << "Usage: lox [script]" << endl;
    } else if (argc == 2) {
        string filename = {argv[1]};
        return runFile(filename, env);
    } else {
        return runPrompt(env);
    }

    return LoxInterpreterResponse::FAILURE;
}
