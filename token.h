#pragma once

typedef enum {
    TOK_NUMBER, TOK_FN, TOK_ANON_FN, TOK_ARROW, TOK_RBRACE, TOK_LBRACE,
    TOK_IDENT, TOK_COLON, TOK_SEMICOLON, TOK_ASSIGN, TOK_EOF, TOK_MINUS
} TokID;

typedef struct {
    const char *start, *end;
} Lex;

typedef struct {
    TokID id;
    Lex lex;
} Token;
