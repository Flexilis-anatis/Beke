#include "chunk.h"

Chunk *empty_chunk(void) {
    Chunk *chunk = calloc(1, sizeof (Chunk));
    return chunk;
}

void free_chunk(Chunk *chunk) {
    v_free(chunk->code);
    v_free(chunk->consts);
    free(chunk);
}

uint16_t get_const_ind(Chunk *chunk, Value val) {
    // Search for an already existing constant of exactly the same value
    for (uint16_t i = 0; i < v_size(chunk->consts); ++i)
        if (val_is(val, chunk->consts[i]))
            return i;

    // If not found, add it now
    v_append(chunk->consts, val);
    return v_size(chunk->consts) - 1;
}

// Not inlining the add_* funcs, because v_append expands to a non-trivial
// peice of code
void add_byte(Chunk *chunk, uint8_t byte) {
    v_append(chunk->code, byte);
}

void add_arg(Chunk *chunk, uint16_t arg) {
    add_byte(chunk, arg >> 8);
    add_byte(chunk, arg & 0xFF);
}

void add_instr(Chunk *chunk, uint8_t op, uint16_t arg) {
    add_byte(chunk, op);
    add_arg(chunk, arg);
}

void add_instr_const(Chunk *chunk, uint8_t op, Value val) {
    add_byte(chunk, op);
    add_const(chunk, val);
}

void add_const(Chunk *chunk, Value val) {
    add_arg(chunk, get_const_ind(chunk, val));
}

inline uint8_t get_byte(uint8_t **ip) {
    return *(*ip)++;
}

inline uint16_t get_arg(uint8_t **ip) {
    return (get_byte(ip) << 8) + get_byte(ip);
}

inline Value get_const(Chunk *chunk, uint8_t **ip) {
    return chunk->consts[get_arg(ip)];
}
