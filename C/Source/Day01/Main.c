#include "Input.h"
#include <Advent/Advent.h>
#include <DayConfig.h>
#include <assert.h>
#include <stdbool.h>
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

typedef struct
{
  int prevValue;
  bool prevValuePresent;
  int count;
} Part1;

typedef struct
{
  int prevSum;
  int window[3];
  bool buildingWindow;
  int buildIndex;
  int count;
} Part2;

#define PART2_INIT                                                                                                     \
  ((Part2){                                                                                                            \
      .prevSum = 0,                                                                                                    \
      .window = {0},                                                                                                   \
      .buildingWindow = true,                                                                                          \
      .buildIndex = 0,                                                                                                 \
      .count = 0,                                                                                                      \
  })

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
  u_strFromUTF8(unicodeInput, (int32_t)inputText_size + 1, &unicodeInputSize, (const char*)inputText,
                (int32_t)inputText_size, &errorCode);
  if (errorCode != U_ZERO_ERROR)
  {
    fprintf(stderr, "%s\n", u_errorName(errorCode));
    return 1;
  }

  UChar* saveptr;
  U_STRING_DECL(delim, "\r\n", 2);
  Part1 part1 = {0};
  Part2 part2 = PART2_INIT;
  for (UChar* iter = u_strtok_r(unicodeInput, delim, &saveptr); iter != NULL; iter = u_strtok_r(NULL, delim, &saveptr))
  {
    int value;
    if (u_sscanf(iter, "%d", &value) == 1)
    {
      if (value > part1.prevValue && part1.prevValuePresent)
      {
        ++part1.count;
      }
      part1.prevValuePresent = true;
      part1.prevValue = value;
      if (part2.buildingWindow)
      {
        part2.window[part2.buildIndex++] = value;
        if (part2.buildIndex == 3)
        {
          part2.buildingWindow = false;
          part2.prevSum = part2.window[0] + part2.window[1] + part2.window[2];
        }
      }
      else
      {
        int prevSum = part2.prevSum;
        part2.prevSum -= part2.window[0];
        part2.window[0] = part2.window[1];
        part2.window[1] = part2.window[2];
        part2.window[2] = value;
        part2.prevSum += value;
        if (part2.prevSum > prevSum)
        {
          ++part2.count;
        }
      }
    }
  }

  printf("%d\n", part1.count);
  printf("%d\n", part2.count);

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
