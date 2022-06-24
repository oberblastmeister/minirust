#ifndef _LOX_DEBUG_H
#define _LOX_DEBUG_H

#include "chunk.h"

void disassemble_chunk(chunk *chunk, const char *name);

int disassemble_instruction(chunk *chunk, int offset);

#endif
