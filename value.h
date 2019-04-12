#pragma once

#include "common.h"

/* Double Floating point storage:
 * Sign bit (1)                      Mantissa (52)
 *    - ----------- ----------------------------------------------------
 *        Exp (11)
 * When all 11 exponent bits are set, it is considered NaN. This gives us 52 Mantissa bits and a sign bit to
 * play with. Since most 64-bit platforms only actually use 48-bit addresses, we can store a pointer and a few
 * nice flags!
 *
 * I'm storing the four extra Mantissa bits as the types, giving me 16 types (and I can do some tricks with
 * the sign bit to get 32 later if I need)
 *
 * Another option for singleton values is to assign all of them one value in the first four Mantissa bits, and
 * then use the rest of the Mantissa bits to tell apart true, false, null, etc.
 *     if ((type = GET_TYPE(value)) == TYPE_SINGLETON) type = GET_VALUE(value)
 * Would do it!
 */

#define POINTER_MASK  281474976710655U // (1 << 52) - 1
#define NAN_MASK      9218868437227405312U // (1 << 63) through (1 << 53)
#define SIGN_MASK     18446744073709551616U // 1 << 63
#define TYPE_MASK     4222124650659840U // (1 << 53) through (1 << 49) or (((1<<4)-1)<<48)
#define MANTISSA_MASK 4503599627370495U // TYPE_MASK | POINTER_MASK

#define VAL_AS(vl,type) ((type)((vl).raw&POINTER_MASK))
#define IS_NUM(vl) (((vl).raw&NAN_MASK)!=NAN_MASK)
#define SET_VALUE(vl,to) ((vl).raw|=((uint64_t)(to)))
#define GET_VALUE(vl) (((vl).raw)&POINTER_MASK)
#define GET_MANTISSA(vl) ((vl).p&MANTISSA_MASK)
#define GET_TYPE(vl) (((vl).raw&TYPE_MASK)>>48)

#define SET_TYPE(vl,to) \
    ((vl).raw = ( \
        ((vl).raw) & ~TYPE_MASK) | NAN_MASK | \
            (((uint64_t)(to)) << 48))

typedef enum {
    TYPE_TRUE, TYPE_FALSE, TYPE_NIL, TYPE_STRING
} Type;

typedef union {
    double num;
    uint64_t raw;
} Value;

typedef struct 
    const char *string;
    int length;
} String;

Value val_d(double val);
bool val_is(Value lhs, Value rhs);
void print_val(Value val);
