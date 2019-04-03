#pragma once
#include "token.h"
#include "common.h"

typedef struct {
    Token last;
    bool peeked;
    const char *start, *end;
} Source;

// Sort of a basic getch and ungetch setup behind the scenes

// Make a new source object
Source init_source(const char *source);

// Quickly get rid of a peeked object; does nothing when no peek is active
void consume(Source *source);

// Get the next token from a source object
Token next_token(Source *source);

// Peek at the next token from a source object. Two peeks in a row return the same token
Token peek_token(Source *source);

// If the next token matches a certain ID, advance over it and return true. Otherwise return false
bool match_token(Source *source, TokID tomatch);
