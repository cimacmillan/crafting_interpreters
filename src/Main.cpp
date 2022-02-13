#include <iostream>

#include "Vector.h"

void mutateX(int& x) {
    x = x * 2;
}

void doubleX(int x) {
    x = x * 2;
}

void doublePoint(int* x) {
    *x = *x * 2;
}

int main() {
    std::cout << "Hello world" << std::endl;
    std::cout << test_func(1) << std::endl;

    int x = 10;
    mutateX(x);

    int y = 10;
    doubleX(x);

    std::cout << x << " " << y << std::endl;

    doublePoint(&y);

    std::cout << y << std::endl;
}

