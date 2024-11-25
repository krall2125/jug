#include <error.h>
#include <stdarg.h>

void complainf(const char *format, ...) {
	fprintf(stderr, "\033[0;31mERROR:\033[0m ");

	va_list args;

	va_start(args, format);

	vfprintf(stderr, format, args);

	va_end(args);
}
