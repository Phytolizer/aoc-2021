#include "Advent/Advent.h"
#include <assert.h>

int main(void)
{
  char* puzzleInput = Advent_ReadPuzzleInput(ADVENT_INPUT_FILE_NAME);
  assert(puzzleInput);
  return 0;
}
