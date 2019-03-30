#include "tests.h"
#include "chunk.h"
#include "scanner.h"
#include <stdio.h>

const TestFn test_fns[] = {
    test_byte_manip,
    test_next_token,
    test_token_manip,
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

char *test_next_token(void) {
    Source source = init_source("fn: fn : ident {= } -> fn name : 53 ->");
    bool passed =
        next_token(&source).id == TOK_ANON_FN &&
        next_token(&source).id == TOK_ANON_FN &&
        next_token(&source).id == TOK_IDENT &&
        next_token(&source).id == TOK_LBRACE &&
        next_token(&source).id == TOK_ASSIGN &&
        next_token(&source).id == TOK_RBRACE &&
        next_token(&source).id == TOK_ARROW &&
        next_token(&source).id == TOK_FN &&
        next_token(&source).id == TOK_IDENT &&
        next_token(&source).id == TOK_COLON &&
        next_token(&source).id == TOK_NUMBER &&
        next_token(&source).id == TOK_ARROW &&
        next_token(&source).id == TOK_EOF &&
        next_token(&source).id == TOK_EOF;

    if (!passed)
        return "next_token(1) failed";
    return NULL;
}

char *test_token_manip(void) {
    Source source = init_source("1 t : ; } { -");
    bool passed =
        next_token(&source).id == TOK_NUMBER &&
        peek_token(&source).id == TOK_IDENT &&
        peek_token(&source).id == TOK_IDENT &&
        next_token(&source).id == TOK_IDENT;

    consume(&source);
    passed &= next_token(&source).id == TOK_COLON &&
              peek_token(&source).id == TOK_SEMICOLON;

    consume(&source);
    passed &= peek_token(&source).id == TOK_RBRACE;
    consume(&source);
    passed &= match_token(&source, TOK_LBRACE) &&
              match_token(&source, TOK_MINUS);

    if (!passed)
        return "Token manipulation functions failed";
    return NULL;
}

bool run_tests(void) {
    int tests = 0, passed = 0;

    TestFn fn = test_fns[tests];
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
