#include <stdarg.h>
#include <stdio.h>
#include<stdlib.h>

void variableOutputFunction(int i, ...) {
    va_list ap;
    int x;
    
    va_start(ap, i);

    printf("%d\n", i);

    x = va_arg(ap, int);

    printf("%d\n", x);

    x = va_arg(ap, int);

    printf("%d\n", x);
    
    va_end(ap);
}

void c7_main() {
    variableOutputFunction(1, 2, 3, 4, 5);

    printf("Input a number and press enter\n");

    int value = 10;
    // scanf("%i", &value);

    printf("You inputted: %d\n", value);

    FILE *fp;
    fp = fopen("./output.txt", "w");
    putc('t', fp);
    putc('e', fp);
    putc('s', fp);
    putc('t', fp);
    fprintf(fp, "test %d", value);
    fputs("This is a whole line", fp);
    fclose(fp);

    fprintf(stdout, "File written\n");
    fprintf(stderr, "This is an error\n");
    // fprintf(stdin, "something");

    int result = system("date > dateOutput.txt");
    printf("The result of running date was %d\n", result);

    double random = ((double) rand() / RAND_MAX);
    printf("Random %f\n", random);
    
}
