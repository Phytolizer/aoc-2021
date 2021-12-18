#include <Advent/Advent.h>
#include <DayConfig.h>
#include <Input.h>
#include <cblas.h>
#include <string.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static void PrintHelp(void);
static void PrintVersion(void);

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

  char** matrix = NULL;
  size_t m = 0;
  size_t n = 0;
  size_t cap = 0;
  size_t rowCap = 0;

  size_t width = 0;

  U_STRING_DECL(delim, "\r\n", 2);
  UChar* saveptr;
  for (UChar* iter = u_strtok_r(unicodeInput, delim, &saveptr); iter != NULL; iter = u_strtok_r(NULL, delim, &saveptr))
  {
    if (m == cap)
    {
      cap = cap ? cap * 2 : 1;
      char** newMatrix = realloc(matrix, cap * sizeof(char*));
      if (newMatrix == NULL)
      {
        printf("Memory allocation failure\n");
        return 4;
      }
      matrix = newMatrix;
      memset(&matrix[m], 0, (cap - m) * sizeof(char*));
    }
    char* row = malloc(rowCap * sizeof(char));
    if (row == NULL)
    {
      printf("Memory allocation failure\n");
      return 4;
    }
    matrix[m] = row;
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
        if (i > n)
        {
          if (n == rowCap)
          {
            rowCap = rowCap ? rowCap * 2 : 1;
            for (size_t i = 0; i < m; i++)
            {
              char* row = realloc(matrix[i], rowCap * sizeof(char));
              if (row == NULL)
              {
                printf("Memory allocation failure\n");
                return 4;
              }
              matrix[i] = row;
            }
          }
          ++n;
        }
        matrix[m][i - 1] = (char)ci;
      }
    }
    ++m;
  }

  char** transposed = malloc(n * sizeof(char*));
  for (size_t i = 0; i < n; ++i)
  {
    transposed[i] = malloc(m * sizeof(char));
    for (size_t j = 0; j < m; ++j)
    {
      transposed[i][j] = matrix[j][i];
    }
  }

  // calculate!
  size_t** counts = malloc(n * sizeof(size_t*));
  uint64_t gamma = 0;
  uint64_t epsilon = 0;
  for (size_t i = 0; i < n; ++i)
  {
    counts[i] = calloc(2, sizeof(size_t));
    for (size_t j = 0; j < m; ++j)
    {
      // NIT: This assumes that the matrix is a rectangle!
      // If there are ANY missing elements, this will likely segfault due to
      // out-of-bounds array access.
      counts[i][transposed[i][j] - '0']++;
    }
    gamma <<= 1;
    gamma |= counts[i][1] > counts[i][0];
    epsilon <<= 1;
    epsilon |= counts[i][0] > counts[i][1];
  }

  printf("%lu\n", gamma * epsilon);

  for (size_t i = 0; i < m; ++i)
  {
    free(matrix[i]);
  }
  free(matrix);
  for (size_t i = 0; i < n; ++i)
  {
    free(transposed[i]);
    free(counts[i]);
  }
  free(transposed);
  free(counts);
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
