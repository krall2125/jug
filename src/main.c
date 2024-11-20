#include <common.h>
#include <bytecode.h>
#include <debug.h>

bool str_endswith(char *str, char *suffix) {
	str = strrchr(str, '.');

	if (str != NULL) {
		return !strcmp(str, suffix);
	}

	return 0;
}

int main(int argc, char **argv) {
	Chunk chunk;

	init_chunk(&chunk);
	write_chunk(&chunk, OP_RETURN);

	disassemble(&chunk, "test");

	free_chunk(&chunk);

	return 0;
	for (int i = 1; i < argc; i++) {
		printf("%s %d\n", argv[i], str_endswith(argv[i], ".jug"));
	}

	return 0;
}
