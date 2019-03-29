#pragma once

#include "vec/vector.h"
#include <stdint.h>

typedef struct {
    vector(uint8_t) code;
} Chunk;

typedef struct {
    uint8_t op;
    uint16_t arg;
} Instr;

Chunk *empty_chunk(void);

void add_byte(Chunk *chunk, uint8_t byte);
void add_arg(Chunk *chunk, uint16_t arg);
void add_instr(Chunk *chunk, uint8_t op, uint16_t arg);
uint8_t get_byte(uint8_t **ip);
uint16_t get_arg(uint8_t **ip);
Instr get_instr(uint8_t **ip);
