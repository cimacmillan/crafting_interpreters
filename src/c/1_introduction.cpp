#include <stdio.h>

/**
 * Examples from C Programming, Kernighan & Ritchie
 * 
 */

#define LOWER 0
#define UPPER 300
#define STEP 20

void print_basic_farenheight() {
    int fahr, celsius;
    int lower = LOWER;
    int upper = UPPER;
    int step = STEP;
    fahr = lower;
    while (fahr <= upper) {
        // Multiply then divide as 5/9 is truncated to 0
        celsius = 5 * (fahr - 32) / 9;

        // %d specifies an integer argument
        // %3d denotes printing it 3 digits wide (right aligned)
        // %6d prints 6 digits wide
        printf("%3d %6d\n", fahr, celsius);
        fahr = fahr + step;
    }
}

void print_float_farenheight() {
    float fahr, celsius;
    int lower = LOWER;
    int upper = UPPER;
    int step = STEP;
    fahr = lower;
    while (fahr <= upper) {
        celsius = (5.0/9.0) * (fahr - 32.0);

        // %3.0f denotes 3 characters wide with no decimal point
        // %6.1f denotes 6 characters wide with 1 decimal point
        printf("%3.0f %6.1f\n", fahr, celsius);
        fahr = fahr + step;
    }
}

// Different type of printf outputs
void print_types() {
    printf("Octal %o, %o, %o\n", 4, 8, 12);
    printf("Hex %x, %x, %x\n", 4, 8, 12);
    printf("Character %c, %c, %c\n", 'a', 'b', 'c');
    printf("String %s\n", "Hello World");
    printf("Special %%\n");
}

void print_input() {
    printf("Press a character then press enter\n");
    // getchar reads one character from input stream (console)
    char c = getchar(); 

    printf("I was given %c\n", c);

    // putchar puts one character to the output stream (console)
    putchar(c);
}


int c_main() {
    // \n is the newline character
    printf("hello, world\n");

    print_basic_farenheight();
    print_float_farenheight();
    print_types();
    print_input();
}


