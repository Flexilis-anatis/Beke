#pragma once

#include <stdint.h>
#include <stdbool.h>

/*
Add flags & masks later.

Actually, maybe just copy them over from BVM?
*/

typedef union {
    double f;
    uint64_t d;
} Value;

Value val_d(double val);
bool val_is(Value lhs, Value rhs);
