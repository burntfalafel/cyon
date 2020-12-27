#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->line = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->line, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  static int ctr = 0;
  static int itr = 0;
  static int prev_line = -1;
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
    chunk->line = GROW_ARRAY(int, chunk->line,
        oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  if(chunk->count==0)
  {
    prev_line = line;
    ctr++;
  }
  else
  {
    if(line == prev_line)
     ctr++;
    else
    {
      prev_line = line;
      ctr=1;
      itr+=2;
    }
  }
  chunk->line[itr] = prev_line;
  chunk->line[itr+1] = ctr;
  chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
