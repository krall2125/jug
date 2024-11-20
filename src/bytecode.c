#include <memory.h>
#include <bytecode.h>

void init_chunk(Chunk *chunk) {
	chunk->count = 0;
	chunk->cap = 0;
	chunk->code = NULL;
}

void write_chunk(Chunk *chunk, u8 byte) {
	if (chunk->count >= chunk->cap) {
		int old = chunk->cap;
		chunk->cap = (chunk->cap <= 0) ? 8 : chunk->cap * 2;
		chunk->code = GROW_ARR(u8, chunk->code, old, chunk->cap);
	}

	chunk->code[chunk->count++] = byte;
}

void free_chunk(Chunk *chunk) {
	FREE_ARR(u8, chunk->code, chunk->cap);
	init_chunk(chunk);
}
