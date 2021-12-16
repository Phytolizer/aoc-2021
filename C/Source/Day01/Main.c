#include "Input.h"
#include <Advent/Advent.h>
#include <DayConfig.h>
#include <assert.h>
#include <stdio.h>

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
