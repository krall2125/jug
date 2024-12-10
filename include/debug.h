#pragma once

#include <bytecode.h>

void disassemble(Chunk *chunk, char *name);
int disassemble_instruction(Chunk *chunk, int offset);
