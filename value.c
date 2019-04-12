#include "value.h"

Value val_d(double val) {
    Value v = {.num = val};
    return v;
}

bool val_is(Value l, Value r) {
    return l.raw == r.raw;
}

void print_val(Value val) {
    printf("%e", val.num);
}
