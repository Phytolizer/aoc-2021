#include "Input.h"
#include <Advent/Advent.h>
#include <DayConfig.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unicode/umachine.h>
#include <unicode/urename.h>
#include <unicode/ustdio.h>
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

  UChar* unicodeInput = malloc((inputText_size + 1) * sizeof(UChar));
  int32_t unicodeInputSize;
  UErrorCode errorCode = U_ZERO_ERROR;
  u_strFromUTF8(unicodeInput, (int32_t)inputText_size, &unicodeInputSize, (const char*)inputText,
                (int32_t)inputText_size, &errorCode);
  unicodeInput[inputText_size] = 0;
  if (errorCode != U_ZERO_ERROR && errorCode != U_STRING_NOT_TERMINATED_WARNING)
  {
    fprintf(stderr, "%s\n", u_errorName(errorCode));
    return 1;
  }

  UChar* saveptr;
  U_STRING_DECL(delim, "\n", 1);
  U_STRING_INIT(delim, "\n", 1);
  int prevValue = -1;
  int count = 0;
  for (UChar* iter = u_strtok_r(unicodeInput, delim, &saveptr); iter != NULL; iter = u_strtok_r(NULL, delim, &saveptr))
  {
    int value;
    if (u_sscanf(iter, "%d", &value) == 1)
    {
      if (value > prevValue && prevValue != -1)
      {
        ++count;
      }
      prevValue = value;
    }
  }

  printf("%d\n", count);

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
