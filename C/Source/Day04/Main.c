#include <Advent/Advent.h>
#include <DayConfig.h>
#include <Input.h>
#include <stdio.h>

void PrintHelp(void);
void PrintVersion(void);

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
