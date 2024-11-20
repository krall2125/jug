#include <common.h>
#include <stdio.h>

char *read_file(const char *path) {
	FILE *file = fopen(path, "r");

	if (file == NULL) {
		fprintf(stderr, "Fatal error: Can't open file %s\n", path);
		return NULL;
	}

	fseek(file, 0L, SEEK_END);

	i32 bytecount = ftell(file);

	rewind(file);

	char *bytes = malloc(sizeof(char) * (bytecount + 1));

	size_t bytes_read = fread(bytes, sizeof(char), bytecount, file);

	bytes[bytes_read] = '\0';

	fclose(file);

	return bytes;
}
