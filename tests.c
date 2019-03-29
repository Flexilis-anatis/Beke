#include "tests.h"
#include "chunk.h"
#include <stdio.h>

const TestFn test_fns[] = {
    test_byte_manip,
    NULL
};

char *test_byte_manip(void) {
    Chunk *chunk = empty_chunk();

    const uint8_t BYTE1 = 123;
    const uint8_t BYTE2 = 53;
    const uint16_t ARG1 = 483;
    const uint8_t OP = 13;
    const uint16_t ARG2 = 1780;

    add_byte(chunk, BYTE1);
    add_byte(chunk, BYTE2);
    add_arg(chunk, ARG1);
    add_instr(chunk, OP, ARG2);

    uint8_t *ip = chunk->code;

    bool get_byte_works =
        get_byte(&ip) == BYTE1 &&
        get_byte(&ip) == BYTE2;

    bool get_arg_works = get_arg(&ip) == ARG1;

    Instr instr = get_instr(&ip);
    bool get_instr_works =
        instr.op == OP &&
        instr.arg == ARG2;

    if (!get_byte_works) {
        return "get_byte(1) or add_byte(1) failed";
    } else if (!get_arg_works) {
        return "get_arg(1) or add_arg(1) failed";
    } else if (!get_instr_works) {
        return "get_instr(1) or add_instr(1) failed";
    } else {
        return NULL;
    }
}

bool run_tests(void) {
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
    return passed == tests;
}
