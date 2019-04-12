#include "bceval.h"
#include <math.h>

static void print_stack(vector(Value) stack) {
    putchar('[');
    if (v_size(stack)) {
        for (size_t i = 0; i < v_size(stack)-1; ++i)
            print_val(stack[i]);
        print_val(stack[v_size(stack)-1]);
    }
    puts("]");
}

Status eval(Chunk *chunk, Value *ret_val) {
    Runner *runner = calloc(1, sizeof (Runner));
    Frame frame = {.chunk = chunk, .ip = chunk->code};
    v_append(runner->frames, frame);
    runner->ip = chunk->code;

    Status stat = eval_rn(runner);
    if (ret_val)
        *ret_val = *(v_end(runner->stack)-1);

    v_free(runner->frames);
    v_free(runner->stack);
    free(runner);
    return stat;
}

Status eval_rn(Runner *runner) {
    Status stat;
    while ((stat = eval_next(runner)) == ST_OK);
    return stat == ST_DONE ? ST_OK : ST_ERR;
}

#define OP(op, ...) \
    case OP_##op: \
        {__VA_ARGS__}    \
        break;


#define END() v_end(rn->stack)
#define TOP() (END()-1)
#define POP() v_pop_back(rn->stack)

#define OPER(name, op) \
    OP(name,                      \
           POP();                 \
           Value top = *END(),    \
                *opval = TOP();   \
           opval->num op##= top.num;)

#define FRAME() rn->frames[v_size(rn->frames)-1]

Status eval_next(Runner *rn) {
    if (rn->ip == v_end(FRAME().chunk->code))
        return ST_DONE;

    Opcode op = *rn->ip++;

    switch (op) {
    OP(PUSH,
        Value val = get_const(FRAME().chunk, &rn->ip);
        v_append(rn->stack, val);)

    OP(NEG,
        Value *top = v_end(rn->stack)-1;
        top->num *= -1;)
    OPER(ADD, +)
    OPER(SUB, -)
    OPER(DIV, /)
    OPER(MUL, *)
    OP(EXP,
        POP();
        Value top = *END(),
             *opval = TOP();
        opval->num = pow(opval->num, top.num);)
    }

    return ST_OK;
}
