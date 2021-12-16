#include "Advent/Advent.h"
#include <stdio.h>
#include <stdlib.h>

char* NS_(ReadPuzzleInput)(const char* inputFileName)
{
  FILE* fp = fopen(inputFileName, "r");
  if (!fp)
  {
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char* buffer = malloc(size + 1);
  fread(buffer, 1, size, fp);
  buffer[size] = '\0';
  fclose(fp);
  return buffer;
}
