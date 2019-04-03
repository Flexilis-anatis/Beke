#include "scanner.h"
#include <string.h>

Source init_source(const char *source) {
    Source s;
    s.end = s.start = source;
    s.peeked = false;
    return s;
}

static Token token(TokID type, Source *source) {
    Token tok;
    tok.id = type;
    tok.lex.start = source->start;
    tok.lex.end = source->end;
    return tok;
}

#define INRANGE(x,l,h) ((x) >= l && (x) <= h)
#define ISIDENTCHAR(x) (INRANGE((x), 'a', 'z') || \
                        INRANGE((x), 'A', 'Z') || \
                        INRANGE((x), '0', '9') || \
                        (x) == '_')
#define ISDIGIT(x) (INRANGE((x),'0','9') || (x) == '.')

Token parse_number(Source *source) {
    while (ISDIGIT(*source->end))
        ++source->end;

    return token(TOK_NUMBER, source);
}

static Token match_kwd(const char *string, size_t length, Source *source, TokID tok) {
    if (source->start+length+1 == source->end &&
        memcmp(source->start+1, string, length) == 0)
        return token(tok, source);
    return token(TOK_IDENT, source);
}

static Token parse_ident(Source *source) {
    while(ISIDENTCHAR(*source->end))
        ++source->end;

    switch (*source->start) {
        case 'f':
            return match_kwd("n", 1, source, TOK_FN);
    }
    return token(TOK_IDENT, source);
}

// More macro abuse! :D
#define CASE(char, type) case char: return token(TOK_##type, source)
static Token parse_other(Source *source) {
    switch(*source->end++) {
        CASE(';', SEMICOLON);
        CASE(':', COLON);
        CASE('{', LBRACE);
        CASE('}', RBRACE);
        case '=':
            return token(TOK_ASSIGN, source);
        case '-':
            if (*source->end == '>') {
                ++source->end;
                return token(TOK_ARROW, source);
            }
            return token(TOK_MINUS, source);
        default:
            return parse_ident(source);
    }
}
#undef CASE

static Token get_next_token(Source *source) {
    source->start = source->end;

    // Skip over whitespace. Not recording lines for now
    while (*source->end == ' ' || *source->end == '\t' || *source->end == '\n')
        source->start = ++source->end;

    // See if we're at the end
    if (*source->end == '\0')
        return token(TOK_EOF, source);

    if (ISDIGIT(*source->end))
        return parse_number(source);
    return parse_other(source);
}

// This is where the peeking happens
Token next_token_peek_filter(Source *source) {
    if (source->peeked) {
        consume(source);
        return source->last;
    }
    return source->last = get_next_token(source);
}

// Filters out fn: from fn IDENT:. Just convenient
Token next_token(Source *source) {
    Token tok = next_token_peek_filter(source);
    if (tok.id == TOK_FN && peek_token(source).id == TOK_COLON) {
        tok.id = TOK_ANON_FN;
        tok.lex.end = peek_token(source).lex.end;
        consume(source);
    }
    return tok;
}

Token peek_token(Source *source) {
    Token token = next_token(source);
    source->peeked = true;
    return token;
}

void consume(Source *source) {
    source->peeked = false;
}

bool match_token(Source *source, TokID id) {
    if (peek_token(source).id != id)
        return false;
    consume(source);
    return true;
}
