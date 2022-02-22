#include <stdio.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int add(int x, int y) {
    return x + y;
}

// int (*opp)(int, int) = pointer to function that returns int from 2 ints
int doOpp(int (*opp)(int, int), int x, int y) {
    return (*opp)(x, y);
}

void c5_main() {
    int x = 1, y = 2;
    int* pi;

    pi = &x;

    printf("Pointer address 0x%X value %d\n", pi, *pi);

    pi = &y;

    printf("Pointer address 0x%X value %d\n", pi, *pi);

    *pi = 5;

    printf("Pointer address 0x%X value %d\n", pi, *pi);

    printf("X: %d, Y: %d\n", x, y);

    swap(&x, &y);

    printf("X: %d, Y: %d\n", x, y);

    int values[] = {7, 9, 4, 5, 2, 0};
    int* valuePointer = &values[0];

    while (*valuePointer != 0) {
        printf("Value pointer is %d\n", *valuePointer);
        valuePointer++;
    }

    char* strings[] = {
        "Hello world\n",
        "My name is callum\n"
    };

    printf(strings[0]);
    printf(strings[1]);

    // 4 int block of memory
    int twoD[2][2] = {{1, 2}, {3, 4}};

    printf("%d %d %d %d\n", twoD[0][0], twoD[0][1], twoD[1][0], twoD[1][1]);

    // Any number of columns but 3 rows max
    // This takes up 9 ints of memory
    int twoDVariableWasteful[][3] = {
        {1, 2, 3},
        {1, 2}, 
        {1}
    };

    int arrayA[] = {1, 2, 3};
    int arrayB[] = {1, 2};
    int* twoDVariableMemoryEfficient[3] = {
        &arrayA[0],
        &arrayB[0]
    };

    printf("%d \n", twoDVariableMemoryEfficient[0][2]);

    printf("%d \n", doOpp(add, 123, 456));

    // function that returns int pointer
    int* f();

    // Pointer to function that returns int
    int (*pf)();

}