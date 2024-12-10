#include <common.h>
#include <bytecode.h>
#include <debug.h>
#include <lexer.h>

bool str_endswith(char *str, char *suffix) {
	str = strrchr(str, '.');

	if (str != NULL) {
		return !strcmp(str, suffix);
	}

	return 0;
}

int main(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (str_endswith(argv[i], ".jug")) {
			char *contents = read_file(argv[i]);
			
			if (contents == NULL) {
				continue;
			}

			printf("%s:\n%s\n", argv[i], contents);

			Lexer lexer;
			lexer_init(&lexer, contents);

			lexer_lex(&lexer);

			lexer_free(&lexer);

			free(contents);
		}
	}

	return 0;
}
