#pragma once

#include <common.h>

#define GROW_ARR(type, pointer, oldsz, newsz) \
	(type *)reallocate(pointer, sizeof(type) * (oldsz), \
			sizeof(type) * (newsz))

#define FREE_ARR(type, pointer, oldsz) \
	reallocate(pointer, sizeof(type) * (oldsz), 0)

#define ALLOC(type, size) \
	(type *)reallocate(NULL, 0, sizeof(type) * size)

#define FREE(type, oldsz, pointer) \
	reallocate(pointer, oldsz, 0)

void *reallocate(void *pointer, size_t old, size_t newsz);
