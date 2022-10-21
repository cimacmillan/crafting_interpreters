#include <string>
#include <iostream>

#include "CPPLox.h"

using namespace std;

void CPPLox::report(int line, string message) {
    cout << "[line " << line << "] Error: " << message << endl;
}

void CPPLox::fatal(int line, string message) {
    CPPLox::report(line, message);
    exit(1);
}
