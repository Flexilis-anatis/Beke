#include "chunk.h"

Chunk *empty_chunk(void) {
    Chunk *chunk = malloc(sizeof (Chunk));
    chunk->code = NULL;
    return chunk;
}

void add_byte(Chunk *chunk, uint8_t byte) {
    vector_push_back(chunk->code, byte);
}

void add_arg(Chunk *chunk, uint16_t arg) {
    add_byte(chunk, arg >> 8);
    add_byte(chunk, arg & 0xFF);
}

void add_instr(Chunk *chunk, uint8_t op, uint16_t arg) {
    add_byte(chunk, op);
    add_arg(chunk, arg);
}

uint8_t get_byte(uint8_t **ip) {
    return *(*ip)++;
}

uint16_t get_arg(uint8_t **ip) {
    return (get_byte(ip) << 8) + get_byte(ip);
}

Instr get_instr(uint8_t **ip) {
    Instr i;
    i.op = get_byte(ip);
    i.arg = get_arg(ip);
    return i;
}
