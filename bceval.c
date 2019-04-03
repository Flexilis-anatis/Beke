#include "bceval.h"
#include <math.h>

static void print_stack(vector(Value) stack) {
    putchar('[');
    if (v_size(stack)) {
        for (size_t i = 0; i < v_size(stack)-1; ++i)
            printf("%e, ", stack[i].f);
        printf("%e", stack[v_size(stack)-1].f);
    }
    puts("]");
}

StatVal eval(Chunk *chunk) {
    Runner *runner = malloc(sizeof (Runner));
    runner->chunk = chunk;
    runner->ip = chunk->code;
    runner->stack = NULL;

    Status stat;
    while ((stat = eval_next(runner)) == ST_OK)
        print_stack(runner->stack);

    StatVal statval;
    statval.stat = (stat == ST_DONE) ? ST_OK : ST_ERR;
    statval.ret = *(v_end(runner->stack)-1);
    return statval;
}

#define OP(op, ...) \
    case OP_##op: \
        {__VA_ARGS__}    \
        break;

#define OPER(name, op) \
    OP(name,                                 \
           v_pop_back(rn->stack);            \
           Value top = *v_end(rn->stack),    \
                *opval = v_end(rn->stack)-1; \
           opval->f op##= top.f;)

Status eval_next(Runner *rn) {
    if (rn->ip == v_end(rn->chunk->code))
        return ST_DONE;

    Opcode op = *rn->ip++;

    switch (op) {
    OP(PUSH,
        Value val = get_const(rn->chunk, &rn->ip);
        v_append(rn->stack, val);)

    OP(NEG,
        Value *top = v_end(rn->stack)-1;
        top->d *= -1;)
    OPER(ADD, +)
    OPER(SUB, -)
    OPER(DIV, /)
    OPER(MUL, *)
    OP(EXP,
        v_pop_back(rn->stack);
        Value top = *v_end(rn->stack),
             *opval = v_end(rn->stack)-1;
        opval->f = pow(opval->f, top.f);)
    }

    return ST_OK;
}
