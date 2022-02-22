#include <stdio.h>
#include <stdlib.h>

#define OPTION_A 0x1
#define OPTION_B 0x2
#define OPTION_C 0x4

bool isEnabled(int options, int option) {
    return options & option;
}

int c2_main() {
    int value = atoi("-123");

    printf("atoi: %d\n", value);

    int option = OPTION_A | OPTION_B;

    printf("isEnabled Option A %d\n", isEnabled(option, OPTION_A));
    printf("isEnabled Option B %d\n", isEnabled(option, OPTION_B));
    printf("isEnabled Option C %d\n", isEnabled(option, OPTION_C));

    int isEnableA = isEnabled(option, OPTION_A) ? 1 : 0;

    printf("isEnabled %d\n", isEnableA);


    int x = 0;
    // Never do this but interesting to know
    lessthanone:
        if (x < 1) {
            x++;
            goto lessthanone;
        }

    printf("X is %d\n", x);

    return 0;
}
