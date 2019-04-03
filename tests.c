#include "tests.h"
#include "chunk.h"
#include "scanner.h"
#include "bceval.h"

const TestFn test_fns[] = {
    test_byte_manip,
    test_next_token,
    test_token_manip,
    test_arithmatic,
    NULL
};

bool run_tests(void) {
    int tests = 0, passed = 0;

    TestFn fn = test_fns[0];
    while (fn) {
        passed += fn();
        fn = test_fns[++tests];
    }

    printf("%d/%d tests passed\n", passed, tests);
    return passed == tests;
}

#define FAIL(msg) \
    do {                              \
        fprintf(stderr, "%s\n", msg); \
        return false;                 \
    } while (0)

#define FAILF(msg, ...) \
    do {                                        \
        fprintf(stderr, msg "\n", __VA_ARGS__); \
        return false;                           \
    } while (0)


#define CHUNKFAIL(g, c) \
    if (g) {  \
        free_chunk(chunk);                \
        c                                 \
    }

bool test_byte_manip(void) {
    Chunk *chunk = empty_chunk();

    const uint8_t BYTE1 = 123;
    const uint8_t BYTE2 = 53;
    const uint16_t ARG1 = 483;
    const uint8_t OP = 14;
    const uint16_t ARG2 = 3456;

    add_byte(chunk, BYTE1);
    add_byte(chunk, BYTE2);
    add_arg(chunk, ARG1);
    add_instr(chunk, OP, ARG2);

    uint8_t *ip = chunk->code;

    int capture;

    CHUNKFAIL((capture = get_byte(&ip)) == BYTE1,
        FAILF("get_byte(1) failed: expected %d, got %d", BYTE1, capture);)

    CHUNKFAIL((capture = get_byte(&ip)) != BYTE2,
        FAILF("get_byte(1) failed: expected %d, got %d", BYTE2, capture);)

    CHUNKFAIL((capture = get_arg(&ip)) != ARG1,
        FAILF("get_arg(1) failed: expected %d, got %d", ARG1, capture);)

    CHUNKFAIL((capture = get_byte(&ip)) != OP,
        FAILF("get_byte(1) failed: expected %d, got %d", OP, capture);)

    CHUNKFAIL((capture = get_arg(&ip)) != ARG2,
        FAILF("get_arg(1) failed: expected %d, got %d", ARG2, capture);)

    free_chunk(chunk);
    return true;
}

#define TEST(tok) \
    if ((capture = next_token(&source).id) != TOK_##tok) \
        FAILF("Expected TOK_" #tok " (value %d) got value %d", TOK_##tok, capture);

bool test_next_token(void) {
    Source source = init_source("fn: fn : ident {= } -> fn name : 53 ->");
    int capture;

    TEST(ANON_FN);
    TEST(ANON_FN);
    TEST(IDENT);
    TEST(LBRACE);
    TEST(ASSIGN);
    TEST(RBRACE);
    TEST(ARROW);
    TEST(FN);
    TEST(IDENT);
    TEST(COLON);
    TEST(NUMBER);
    TEST(ARROW);

    return true;
}
#undef TEST

bool test_token_manip(void) {
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
        FAIL("Token manipulation functions failed");
    return true;
}

bool test_arithmatic(void) {
    const double value = 3.5 / 7.1 * 9.0 - 2.2;

    Chunk *chunk = empty_chunk();
    add_instr_const(chunk, OP_PUSH, val_d(3.5));
    add_instr_const(chunk, OP_PUSH, val_d(7.1));
    add_byte(chunk,        OP_DIV);
    add_instr_const(chunk, OP_PUSH, val_d(9));
    add_byte(chunk,        OP_MUL);
    add_instr_const(chunk, OP_PUSH, val_d(2.2));
    add_byte(chunk,        OP_SUB);

    StatVal ret = eval(chunk);
    Status status = ret.stat;
    Value result = ret.ret;

    CHUNKFAIL(status == ST_ERR,
        FAILF("Error occured during arithmatic test: '%s'", BCError.message);)

    CHUNKFAIL(result.f != value,
        FAILF("Arithmatic failed; expected %e, got %e", value, result.f);)

    free_chunk(chunk);

    return true;
}
