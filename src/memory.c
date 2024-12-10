#include <memory.h>

void *reallocate(void *ptr, size_t old, size_t newsz) {
	if (newsz == 0) {
		free(ptr);
		return NULL;
	}

	void *result = realloc(ptr, newsz);

	if (result == NULL) {
		fprintf(stderr, "fatal error: buy more ram lol\n");
		exit(1);
	}

	return result;
}
