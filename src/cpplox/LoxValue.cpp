#include "LoxValue.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const LoxValue & value) {
    switch (value.type) {
        case LoxValueType::NUMBER:
            os << value.number;
            break;
        case LoxValueType::STRING:
            // cout << "\"" << *(value.str) << "\"";
            os << *(value.str);
            break;
        case LoxValueType::BOOLEAN:
            os << (value.boolean ? "true" : "false");
            break;
        case LoxValueType::NIL:
            os << "nil";
            break;
    }
    return os;
}

