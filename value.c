#include "value.h"

Value val_d(double val) {
    Value v = {.f = val};
    return v;
}

bool val_is(Value l, Value r) {
    return l.d == r.d;
}
