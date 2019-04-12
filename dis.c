#include "dis.h"

void dis(Chunk *chunk) {
    uint8_t *ip = chunk->code;
    while ((ip += dis_instr(chunk, ip)) != v_end(chunk->code));
}

#define BYTE(name) case OP_##name: printf("%s", #name); break;
int dis_instr(Chunk *chunk, uint8_t *ip) {
    uint8_t *original = ip;

    printf("@%ld: ", ip - chunk->code);
    switch (get_byte(&ip)) {
    BYTE(DIV)
    BYTE(MUL)
    BYTE(EXP)
    BYTE(ADD)
    BYTE(SUB)
    BYTE(NEG)

    case OP_PUSH: {
        Value arg = get_const(chunk, &ip);
        printf("PUSH ");
        print_val(arg);
        break;
    }
    }
    putchar('\n');

    return ip - original;
}
