#include <Advent/Advent.h>
#include <Advent/Vector.h>
#include <DayConfig.h>
#include <Input.h>
#include <stdio.h>
#include <unicode/umachine.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

void PrintHelp(void);
void PrintVersion(void);

VECTOR_DECLARE(I64Vec, int64_t);
VECTOR_IMPLEMENT(I64Vec, int64_t, VECTOR_DESTRUCTOR_NONE(I64Vec));

int main(int argc, char** argv)
{
  CommandLineCommand command;
  if (Advent_ParseCommandLine(argc, argv, &command))
  {
    switch (command)
    {
    case COMMAND_LINE_COMMAND_PRINT_HELP:
      PrintHelp();
      return 0;
    case COMMAND_LINE_COMMAND_PRINT_HELP_ERROR:
      PrintHelp();
      return 1;
    case COMMAND_LINE_COMMAND_PRINT_VERSION:
      PrintVersion();
      return 0;
    default:
      fprintf(stderr, "Unknown command\n");
      return 2;
    }
  }

  UChar* unicodeInput = malloc((inputText_size + 1) * sizeof(UChar));
  int32_t unicodeInputSize = 0;
  UErrorCode errorCode = U_ZERO_ERROR;
  u_strFromUTF8(unicodeInput, (int32_t)(inputText_size + 1), &unicodeInputSize, (const char*)inputText,
                (int32_t)inputText_size, &errorCode);
  if (errorCode != U_ZERO_ERROR)
  {
    fprintf(stderr, "Unicode error: %s\n", u_errorName(errorCode));
    return 3;
  }

  UChar* mainptr;
  U_STRING_DECL(delim, "\n\n", 2);
  UChar* drawsStr = u_strtok_r(unicodeInput, delim, &mainptr);

  I64Vec draws;
  I64Vec_init(&draws);
  UChar* drawSavePtr;
  U_STRING_DECL(commaDelim, ",", 1);
  for (UChar* drawStr = u_strtok_r(drawsStr, commaDelim, &drawSavePtr); drawStr;
       drawStr = u_strtok_r(NULL, commaDelim, &drawSavePtr))
  {
    int draw;
    if (u_sscanf(drawStr, "%d", &draw) == 1)
    {
      I64Vec_push(&draws, draw);
    }
  }

  I64Vec_deinit(&draws);
  free(unicodeInput);
  return 0;
}

void PrintHelp(void)
{
  printf("Usage: %s [options]\n", ADVENT_DAY_NAME);
  printf("Options:\n");
  printf(" -h,--help    Print this help message\n");
  printf(" -V,--version Print version information\n");
}

void PrintVersion(void)
{
  printf("%s v%s\n", ADVENT_DAY_NAME, PROJECT_VERSION);
}
