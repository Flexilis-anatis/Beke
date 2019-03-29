#include "tests.h"
#include <stdio.h>

int main(int argc, char **argv) {
    int tests = 0, passed = 0;

    TestFn fn = test_fns[0];
    while (fn) {
        char *result = fn();
        if (result) {
            printf("%s\n", result);
        } else {
            ++passed;
        }

        fn = test_fns[++tests];
    }

    printf("%d/%d tests passed\n", passed, tests);

    return 0;
}
