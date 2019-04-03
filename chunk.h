#pragma once

#include "common.h"

typedef enum {
    OP_PUSH,
    // Arithmatic
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_NEG, OP_EXP
} Opcode;

typedef struct {
    vector(Value) consts;
    vector(uint8_t) code;
} Chunk;

Chunk *empty_chunk(void);
void free_chunk(Chunk *chunk);

uint16_t get_const_ind(Chunk *chunk, Value val);

void add_byte(Chunk *chunk, uint8_t byte);
void add_arg(Chunk *chunk, uint16_t arg);
void add_instr(Chunk *chunk, uint8_t op, uint16_t arg);
void add_instr_const(Chunk *chunk, uint8_t op, Value val);
void add_const(Chunk *chunk, Value val);

uint8_t get_byte(uint8_t **ip);
uint16_t get_arg(uint8_t **ip);
Value get_const(Chunk *chunk, uint8_t **ip);
