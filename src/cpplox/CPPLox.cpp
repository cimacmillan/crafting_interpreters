#include <string>
#include <iostream>

#include "CPPLox.h"

using namespace std;

namespace CPPLox {

    void report(int line, string message) {
        cout << "[line " << line << "] Error: " << message << endl;
    }

}


