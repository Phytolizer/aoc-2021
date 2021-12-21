#include <Advent/Advent.h>
#include <Advent/Vector.h>
#include <DayConfig.h>
#include <Input.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unicode/umachine.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>

void PrintHelp(void);
void PrintVersion(void);

VECTOR_DECLARE(I64Vec, int64_t);
VECTOR_IMPLEMENT(I64Vec, int64_t, VECTOR_DESTRUCTOR_NONE(I64Vec));

typedef struct
{
  int64_t value;
  bool marked;
} BingoBoardSlot;

typedef struct
{
  BingoBoardSlot b[5 * 5];
  bool won;
} BingoBoard;

VECTOR_DECLARE(BingoBoardVec, BingoBoard);
VECTOR_IMPLEMENT(BingoBoardVec, BingoBoard, VECTOR_DESTRUCTOR_NONE(BingoBoardVec));

bool BingoBoard_checkWin(BingoBoard* b);

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
  U_STRING_DECL(delim, "\r\n", 2);
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

  U_STRING_DECL(doubleNewlineDelim, "\n\n", 2);
  BingoBoardVec boards;
  BingoBoardVec_init(&boards);
  UChar* bingoBoard = mainptr + 1;
  for (UChar* doubleNewline = u_strstr(bingoBoard, doubleNewlineDelim); doubleNewline;
       doubleNewline = u_strstr(doubleNewline + 2, doubleNewlineDelim))
  {
    *doubleNewline = 0;
    BingoBoard board = {0};
    UChar* bingoSavePtr;
    size_t i = 0;
    for (UChar* bingoLineStr = u_strtok_r(bingoBoard, delim, &bingoSavePtr); bingoLineStr;
         bingoLineStr = u_strtok_r(NULL, delim, &bingoSavePtr))
    {
      assert(i < 25);
      assert(u_sscanf(bingoLineStr, "%d %d %d %d %d", &board.b[i].value, &board.b[i + 1].value, &board.b[i + 2].value,
                      &board.b[i + 3].value, &board.b[i + 4].value) == 5);
      i += 5;
    }
    BingoBoardVec_push(&boards, board);
    bingoBoard = doubleNewline + 2;
  }

  // End input parsing
  size_t myBoard = SIZE_MAX;
  size_t winningCallIdx = SIZE_MAX;
  size_t nwon = 0;
  size_t lastBoard = SIZE_MAX;
  size_t lastWinningCallIdx = SIZE_MAX;

  for (size_t i = 0; i < draws.length; ++i)
  {
    for (size_t j = 0; j < boards.length; ++j)
    {
      // mark board
      if (!boards.data[j].won)
      {
        for (size_t k = 0; k < 25; ++k)
        {
          if (boards.data[j].b[k].value == draws.data[i])
          {
            boards.data[j].b[k].marked = true;
          }
        }

        if (BingoBoard_checkWin(&boards.data[j]))
        {
          boards.data[j].won = true;
          ++nwon;
          if (nwon == boards.length)
          {
            printf("Last winner is %zu, won on draw %zu (%ld)\n", j, i, draws.data[i]);
            lastBoard = j;
            lastWinningCallIdx = i;
          }
          if (myBoard == SIZE_MAX)
          {
            printf("Selected board %zu, won on draw %zu (%ld)\n", j, i, draws.data[i]);
            myBoard = j;
            winningCallIdx = i;
          }
        }
      }
    }
  }

  int64_t unmarkedSum = 0;
  for (size_t i = 0; i < 25; ++i)
  {
    if (!boards.data[myBoard].b[i].marked)
    {
      unmarkedSum += boards.data[myBoard].b[i].value;
    }
  }

  printf("Part 1: %ld\n", unmarkedSum * draws.data[winningCallIdx]);

  unmarkedSum = 0;
  for (size_t i = 0; i < 25; ++i)
  {
    if (!boards.data[lastBoard].b[i].marked)
    {
      unmarkedSum += boards.data[lastBoard].b[i].value;
    }
  }

  printf("Part 2: %ld\n", unmarkedSum * draws.data[lastWinningCallIdx]);

  BingoBoardVec_deinit(&boards);
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

bool BingoBoard_checkWin(BingoBoard* b)
{
  // check vertical lines
  for (size_t i = 0; i < 5; ++i)
  {
    bool thisLineWin = true;
    for (size_t j = i; j < 25; j += 5)
    {
      if (!b->b[j].marked)
      {
        thisLineWin = false;
        break;
      }
    }
    if (thisLineWin)
    {
      return true;
    }
  }

  // check horizontal lines
  for (size_t i = 0; i < 25; i += 5)
  {
    bool thisLineWin = true;
    for (size_t j = i; j < i + 5; ++j)
    {
      if (!b->b[j].marked)
      {
        thisLineWin = false;
        break;
      }
    }
    if (thisLineWin)
    {
      return true;
    }
  }
  return false;
}
