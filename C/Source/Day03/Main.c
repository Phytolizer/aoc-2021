#include <Advent/Advent.h>
#include <DayConfig.h>
#include <Input.h>
#include <string.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static void PrintHelp(void);
static void PrintVersion(void);

typedef struct
{
  size_t c[2];
} Count;

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

  Count* counts = NULL;
  size_t width = 0;

  U_STRING_DECL(delim, "\r\n", 2);
  UChar* saveptr;
  for (UChar* iter = u_strtok_r(unicodeInput, delim, &saveptr); iter != NULL; iter = u_strtok_r(NULL, delim, &saveptr))
  {
    size_t i = 0;
    while (true)
    {
      UChar32 ci;
      U16_NEXT(iter, i, -1, ci);
      if (ci == 0)
      {
        break;
      }
      if (ci == '0' || ci == '1')
      {
        if (width < i || counts == NULL)
        {
          Count* newCounts = realloc(counts, sizeof(Count) * (i + 1));
          if (newCounts == NULL)
          {
            fprintf(stderr, "Memory allocation failure\n");
            return 4;
          }
          counts = newCounts;
          memset(&counts[i], 0, sizeof(Count));
          width = i;
        }
        counts[i].c[ci - '0']++;
      }
    }
  }

  uint64_t gamma = 0;
  uint64_t epsilon = 0;
  for (size_t i = 0; i < width; ++i)
  {
    gamma <<= 1;
    gamma |= counts[i].c[1] > counts[i].c[0];
    printf("%lx\n", gamma);
    epsilon <<= 1;
    epsilon |= !(gamma & 1);
  }

  printf("%lu\n", gamma * epsilon);

  free(unicodeInput);
}

static void PrintHelp(void)
{
  PrintVersion();
  printf("Usage: %s [options]\n", ADVENT_DAY_NAME);
  puts("Options:");
  puts(" -h,--help    Print this help message");
  puts(" -V,--version Print the program version");
}

static void PrintVersion(void)
{
  printf("%s v%s\n", ADVENT_DAY_NAME, PROJECT_VERSION);
}
