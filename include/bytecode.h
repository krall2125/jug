#pragma once

#include <common.h>

typedef enum {
	OP_RETURN,
} BytecodeOp;

typedef struct {
	size_t count;
	size_t cap;
	u8 *code;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, u8 byte);
void free_chunk(Chunk *chunk);
