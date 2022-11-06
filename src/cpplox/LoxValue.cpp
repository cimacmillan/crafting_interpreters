#include "LoxValue.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const LoxValue & value) {
    switch (value.type) {
        case LoxValueType::NUMBER:
            cout << value.number;
            break;
        case LoxValueType::STRING:
            // cout << "\"" << *(value.str) << "\"";
            cout << *(value.str);
            break;
        case LoxValueType::BOOLEAN:
            cout << (value.boolean ? "true" : "false");
            break;
        case LoxValueType::NIL:
            cout << "nil";
            break;
    }
    return os;
}

