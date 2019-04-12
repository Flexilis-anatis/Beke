#include "mem.h"

int allocedSize = 0;
int next_gc = 1024;
#define GC_GROW_RATE 2

void *beke_realloc(MemLoc ptr, int old, int new) {
    allocedSize += new - old;

    if (new > old && allocedSize <= next_gc) {
        run_gc();
    }

    if (!new) {
        free(ptr);
        return NULL;
    }

    else if (!ptr) {
        return malloc(new);
    }

    else {
        return realloc(ptr, new);
    }
}
