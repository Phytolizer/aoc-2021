#include <Advent/Advent.h>
#include <DayConfig.h>
#include <Input.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static void PrintHelp(void);
static void PrintVersion(void);

typedef struct
{
  long x;
  long y;
} Part1;

typedef struct
{
  long x;
  long y;
  long aim;
} Part2;

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
      fprintf(stderr, "Unrecognized command\n");
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

  Part1 part1 = {0};
  Part2 part2 = {0};

  UChar* saveptr = NULL;
  U_STRING_DECL(delim, "\r\n", 2);
  for (UChar* iter = u_strtok_r(unicodeInput, delim, &saveptr); iter != NULL; iter = u_strtok_r(NULL, delim, &saveptr))
  {
    int delta = 0;
    if (u_sscanf(iter, "forward %d", &delta) == 1)
    {
      part1.x += delta;
      part2.x += delta;
      part2.y += part2.aim * delta;
    }
    else if (u_sscanf(iter, "up %d", &delta) == 1)
    {
      part1.y -= delta;
      part2.aim -= delta;
    }
    else if (u_sscanf(iter, "down %d", &delta) == 1)
    {
      part1.y += delta;
      part2.aim += delta;
    }
    else
    {
      u_printf("weird input line: %S\n", iter);
    }
  }

  printf("%ld\n", part1.x * part1.y);
  printf("%ld\n", part2.x * part2.y);

  free(unicodeInput);
  return 0;
}

void PrintHelp(void)
{
  PrintVersion();
  printf("Usage: %s [options]\n", ADVENT_DAY_NAME);
  puts("Options:");
  puts(" -h, --help Print this help message");
  puts(" -V, --version Print the program version");
}

void PrintVersion(void)
{
  printf("Advent of Code %s v%s\n", ADVENT_DAY_NAME, PROJECT_VERSION);
}
