#pragma once

#include "common.h"

typedef struct GCObj {
    bool is_dark;

}

#define ALLOC(size) \
    beke_realloc(NULL, 0, size)

#define REALLOC(ptr, old, new) \
    beke_realloc(ptr, old, new)

#define FREE(ptr, size) \
    beke_realloc(ptr, size, 0)

void *beke_realloc(void *ptr, size_t old, size_t new);
