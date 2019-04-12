#pragma once
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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Including last in case they need anything above
#include "vec/vector.h"
#include "value.h"
