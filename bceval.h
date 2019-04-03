#pragma once
#include "chunk.h"

/* Bytecode Evaluator
 *
 * Given a set of instructions, executes them and returns status
 */

struct {
    const char *message;
} BCError;

typedef enum {
    ST_OK, ST_DONE, ST_ERR
} Status;

typedef struct {
    Status stat;
    Value ret;
} StatVal;

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    vector(Value) stack;
} Runner;

// Returns ST_OK on exit, ST_ERR on failure, as well as top of stack
StatVal eval(Chunk *chunk);
// Returns ST_OK on exit, ST_DONE when end is reached, ST_ERR on failure
Status eval_next(Runner *runner);
