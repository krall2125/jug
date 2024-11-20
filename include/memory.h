#pragma once

#include <common.h>

#define GROW_ARR(type, pointer, oldsz, newsz) \
	(type *)reallocate(pointer, sizeof(type) * (oldsz), \
			sizeof(type) * (newsz))

#define FREE_ARR(type, pointer, oldsz) \
	reallocate(pointer, sizeof(type) * (oldsz), 0)

void *reallocate(void *pointer, size_t old, size_t newsz);
