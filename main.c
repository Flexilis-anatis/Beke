#include "tests.h"
#include <stdio.h>

int main(int argc, char **argv) {
    bool result = run_tests();

    return !result; // C-truthiness -> shell-truthiness
}
