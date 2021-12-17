#include "Input.h"
#include <Advent/Advent.h>
#include <DayConfig.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unicode/umachine.h>
#include <unicode/urename.h>
#include <unicode/ustring.h>
#include <unicode/utf8.h>
#include <unicode/utypes.h>

static void PrintHelp(void);
static void PrintVersion(void);

int main(int argc, char** argv)
{
  CommandLineCommand command;
  bool shouldReturn = Advent_ParseCommandLine(argc, argv, &command);
  if (shouldReturn)
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
      assert(false && "unrecognized command");
    }
  }

  UChar* unicodeInput = malloc(inputText_size * sizeof(UChar));
  int32_t unicodeInputSize;
  UErrorCode errorCode = U_ZERO_ERROR;
  u_strFromUTF8(unicodeInput, (int32_t)inputText_size, &unicodeInputSize, (const char*)inputText,
                (int32_t)inputText_size, &errorCode);
  if (errorCode != U_ZERO_ERROR && errorCode != U_STRING_NOT_TERMINATED_WARNING)
  {
    fprintf(stderr, "%s\n", u_errorName(errorCode));
    return 1;
  }

  free(unicodeInput);
  return 0;
}

static void PrintHelp(void)
{
  printf("Usage: %s [options]\n", ADVENT_DAY_NAME);
  printf("Options:\n");
  printf(" -h, --help Print this help message\n");
  printf(" -V, --version Print version information\n");
}

static void PrintVersion(void)
{
  printf("%s v%s\n", ADVENT_DAY_NAME, PROJECT_VERSION);
}
