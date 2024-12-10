#include <common.h>
#include <stdio.h>
#include <debug.h>

void disassemble(Chunk *chunk, char *name) {
	printf("<==+==> %s <==+==>\n", name);

	for (int i = 0; i < chunk->count;) {
		i = disassemble_instruction(chunk, i);
	}
}

int disassemble_instruction(Chunk *chunk, int offset) {
	printf("%04d ", offset);

	u8 instr = chunk->code[offset];

	switch (instr) {
		case OP_RETURN: {
			printf("OP_RETURN\n");
			return offset + 1;
		}
		default: {
			printf("Unknown opcode %d\n", instr);
			return offset + 1;
		}
	}
}
