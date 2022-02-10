#include <Advent/Files.h>
#include <Config.h>
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

  free(fileContents.data);
  return 0;
}
