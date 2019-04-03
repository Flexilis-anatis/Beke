#pragma once
#include "vec/vector.h"
#include "value.h"

// Librarys we'll use a lot
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define DEBUG

#ifdef DEBUG
#define ASSERT(cond, msg)   \
    do {                                        \
        if (cond) break;                        \
        fprintf(stderr,                         \
           #cond": Assertion failed: %s", msg); \
        exit(-1);                               \
    } while (0)
#else
#define ASSERT(c,m)
#endif
