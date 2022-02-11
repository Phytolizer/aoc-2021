#include <Advent/Core.h>
#include <Advent/Files.h>
#include <Config.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  const char* filePath = CMAKE_DAY_DIR "/Input.txt";
  if (argc > 1)
  {
    filePath = argv[1];
  }

  AdventString fileContents = adventSlurpFile(filePath);
  if (fileContents.data == NULL)
  {
    return 1;
  }

  AdventSViewPair pair = adventSViewTokenize(ADVENT_STRING_AS_VIEW(fileContents), ADVENT_SVIEW_C("\n"));
  while (pair.first.length != 0)
  {
    printf("%.*s\n", (int)pair.first.length, pair.first.data);
    pair = adventSViewTokenize(pair.second, ADVENT_SVIEW_C("\n"));
  }

  free(fileContents.data);
  return 0;
}
