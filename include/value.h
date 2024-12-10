#pragma once

#include <common.h>

typedef enum {
	TYPE_U8,
	TYPE_U16,
	TYPE_U32,
	TYPE_U64,
	TYPE_I8,
	TYPE_I16,
	TYPE_I32,
	TYPE_I64,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_MAP,
} ValueType;

typedef double Value;
