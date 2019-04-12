#include "comp.h"

typedef enum {
    PREC_NONE,
    PREC_FN,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_EXP,
    PREC_PAREN,
    PREC_ATOM
} Prec;

#define ERROR(msg) do { \
    printf("%s", msg);  \
    exit(-11);          \
}while(0)

#define COMP(name) name(source, chunk)
#define DECL_ParseFn(name) static void name(Source *source, Chunk *chunk)

typedef void (*ParseFn)(Source *, Chunk *);
DECL_ParseFn(expression);
DECL_ParseFn(binary);
DECL_ParseFn(unary);
DECL_ParseFn(paren);
DECL_ParseFn(number);
DECL_ParseFn(anon_fn);
DECL_ParseFn(ident);

typedef struct {
    ParseFn prefix, postfix;
    Prec prec;
    bool leftAssoc;
} ParseRule;
/*
TOK_NUMBER, TOK_FN, TOK_ANON_FN, TOK_ARROW, TOK_RBRACE, TOK_LBRACE,
TOK_IDENT, TOK_COLON, TOK_SEMICOLON, TOK_ASSIGN, TOK_EOF, TOK_MINUS
*/
#define NULL_RULE {NULL, NULL, PREC_NONE, true}
static const ParseRule rules[] = {
    [TOK_NUMBER]    = {number, NULL, PREC_ATOM, true},
    [TOK_FN]        = NULL_RULE,
    [TOK_ANON_FN]   = {anon_fn, NULL, PREC_FN, true},
    [TOK_ARROW]     = NULL_RULE,
    [TOK_RBRACE]    = NULL_RULE,
    [TOK_LBRACE]    = NULL_RULE,
    [TOK_IDENT]     = {ident, NULL, PREC_ATOM, true},
    [TOK_COLON]     = NULL_RULE,
    [TOK_SEMICOLON] = NULL_RULE,
    [TOK_ASSIGN]    = NULL_RULE, // temp
    [TOK_MINUS]     = {unary, binary, PREC_TERM, true},
    [TOK_PLUS]      = {unary, binary, PREC_TERM, true},
    [TOK_STAR]      = {NULL, binary, PREC_FACTOR, true},
    [TOK_SLASH]     = {NULL, binary, PREC_FACTOR, true},
    [TOK_LPAREN]    = {paren, NULL, PREC_PAREN, true},
    [TOK_RPAREN]    = NULL_RULE,
    [TOK_STRING]    = {string, NULL, PREC_ATOM, true},
    [TOK_EOF]       = NULL_RULE
};

Chunk *compile(Source *source) {
    Chunk *chunk = empty_chunk();
    next_token(source);
    COMP(expression);
    if (cur_token(source).id != TOK_EOF)
        ERROR("Expected end of expression");
    return chunk;
}

void parse_prec(Source *source, Chunk *chunk, Prec prec) {
    next_token(source);

    ParseFn prefix = rules[last_token(source).id].prefix;
    if (prefix == NULL)
        ERROR("Expected valid (sub-)expression");
    COMP(prefix);

    while (prec <= rules[cur_token(source).id].prec) {
        next_token(source);
        COMP(rules[last_token(source).id].postfix);
    }
}

DECL_ParseFn(expression) {
    parse_prec(source, chunk, PREC_FN);
}

DECL_ParseFn(binary) {
    TokID id = last_token(source).id;
    ParseRule rule = rules[id];
    parse_prec(source, chunk, rule.prec + rule.leftAssoc);

    u8 op;
    switch (id) {
    case TOK_MINUS:
        op = OP_SUB;
        break;
    case TOK_PLUS:
        op = OP_ADD;
        break;
    case TOK_SLASH:
        op = OP_DIV;
        break;
    case TOK_STAR:
        op = OP_MUL;
        break;
    default:;
    }
    add_byte(chunk, op);
}

DECL_ParseFn(unary) {
    TokID id = last_token(source).id;
    parse_prec(source, chunk, PREC_UNARY);
    u8 op;
    switch (id) {
    case TOK_MINUS:
        op = OP_NEG;
        break;
    default:;
    }
    add_byte(chunk, op);
}

DECL_ParseFn(paren) {
    COMP(expression);
    if (cur_token(source).id != TOK_RPAREN)
        ERROR("Expected closing parenthesis");
    next_token(source);
}

DECL_ParseFn(number) {
    const char *numstring = last_token(source).lex.start;
    double number = strtod(numstring, NULL);
    add_instr_const(chunk, OP_PUSH, val_d(number));
}

DECL_ParseFn(anon_fn) {
    (void)source;
    (void)chunk;
}

DECL_ParseFn(ident) {
    (void)source;
    (void)chunk;
}
