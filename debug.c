#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk,
                               int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

int getLine(Chunk* chunk, int offset, int *flag_prev)
{
  static int old_offset = 0;
  static int itr = 0;
  static int i = 0;
  if(old_offset != offset && i<chunk->line[itr+1])
  {
    *flag_prev = 1;
  }
  else if (old_offset != offset)
  {
    i = 0;
    itr+=2;
    *flag_prev = 0;
  }
  i = (offset-old_offset)+i+1; //some opcodes use more than 1 offset. dissambler should take care of it.
  if(old_offset + offset == 0)
    i++;
  old_offset = offset;
  return chunk->line[itr];
}

int disassembleInstruction(Chunk* chunk, int offset) {
  printf("%04d ", offset);
  int flag_prev = 0;

  int chunk_line = getLine(chunk, offset, &flag_prev);
  if (offset > 0 &&
      flag_prev == 1) {
    printf("   | ");
  } else {
    printf("%4d ", chunk_line);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}
