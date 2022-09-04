#pragma once

#include "chunk.h"

void disassemble_chunk(chunk *chunk, const char *name);

int disassemble_instruction(chunk *chunk, int offset);
