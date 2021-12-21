#include <Advent/Advent.h>
#include <Advent/Hash.h>
#include <Advent/Vector.h>
#include <DayConfig.h>
#include <Input.h>
#include <stdint.h>
#include <stdio.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>

static void PrintHelp(void);
static void PrintVersion(void);

typedef struct
{
  int x;
  int y;
} Coordinate;

Coordinate* Coordinate_new(int x, int y)
{
  Coordinate c = {x, y};
  return Advent_MemDup(&c, sizeof(Coordinate));
}

typedef struct
{
  Coordinate c[2];
} LineSegment;

VECTOR_DECLARE(LineSegmentVec, LineSegment);
VECTOR_IMPLEMENT(LineSegmentVec, LineSegment, VECTOR_DESTRUCTOR_NONE(LineSegmentVec));

HASH_DECLARE(PointMap, size_t);
HASH_IMPLEMENT(PointMap, size_t, HASH_DESTRUCTOR_KEY(PointMap, free), HASH_MAX_LOAD_DEFAULT);

int IntLerp(int a, int min, int value)
{
  return a + value - min;
}

void PointMap_populate(PointMap* map, LineSegmentVec* segs, bool override)
{
  for (size_t i = 0; i < segs->length; ++i)
  {
    if (segs->data[i].c[0].x == segs->data[i].c[1].x)
    {
      if (segs->data[i].c[0].y > segs->data[i].c[1].y)
      {
        int temp = segs->data[i].c[0].y;
        segs->data[i].c[0].y = segs->data[i].c[1].y;
        segs->data[i].c[1].y = temp;
      }
      for (int y = segs->data[i].c[0].y; y <= segs->data[i].c[1].y; ++y)
      {
        PointMap_key k = {
            .key = Coordinate_new(segs->data[i].c[0].x, y),
            .len = sizeof(Coordinate),
        };
        size_t* v = PointMap_lookup(map, k);
        if (v == NULL)
        {
          PointMap_insert(map, k, 1);
        }
        else
        {
          free(k.key);
          (*v)++;
        }
      }
    }
    else if (segs->data[i].c[0].y == segs->data[i].c[1].y || override)
    {
      if (segs->data[i].c[0].x > segs->data[i].c[1].x)
      {
        int temp = segs->data[i].c[0].x;
        segs->data[i].c[0].x = segs->data[i].c[1].x;
        segs->data[i].c[1].x = temp;
      }
      if (segs->data[i].c[0].y > segs->data[i].c[1].y)
      {
        int temp = segs->data[i].c[0].y;
        segs->data[i].c[0].y = segs->data[i].c[1].y;
        segs->data[i].c[1].y = temp;
      }
      for (int x = segs->data[i].c[0].x; x <= segs->data[i].c[1].x; ++x)
      {
        int y = IntLerp(segs->data[i].c[0].y, segs->data[i].c[0].x, x);
        PointMap_key k = {
            .key = Coordinate_new(x, segs->data[i].c[0].y),
            .len = sizeof(Coordinate),
        };
        size_t* v = PointMap_lookup(map, k);
        if (v == NULL)
        {
          PointMap_insert(map, k, 1);
        }
        else
        {
          free(k.key);
          (*v)++;
        }
      }
    }
  }
}

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

  UChar* saveptr;
  U_STRING_DECL(delim, "\r\n", 2);
  LineSegmentVec segs;
  LineSegmentVec_init(&segs);
  for (UChar* line = u_strtok_r(unicodeInput, delim, &saveptr); line != NULL; line = u_strtok_r(NULL, delim, &saveptr))
  {
    LineSegment seg;
    if (u_sscanf(line, "%d,%d -> %d,%d", &seg.c[0].x, &seg.c[0].y, &seg.c[1].x, &seg.c[1].y) == 4)
    {
      LineSegmentVec_push(&segs, seg);
    }
  }

  // Part 1. Only consider H/V lines.
  PointMap p1map;
  PointMap_init(&p1map);
  PointMap_populate(&p1map, &segs, false);

  size_t count = 0;
  for (size_t i = 0; i < p1map.capacity; ++i)
  {
    if (p1map.buckets[i].present && p1map.buckets[i].value > 1)
    {
      ++count;
    }
  }
  PointMap_deinit(&p1map);

  PointMap p2map;
  PointMap_init(&p2map);
  PointMap_populate(&p2map, &segs, true);
  size_t count2 = 0;
  for (size_t i = 0; i < p2map.capacity; ++i)
  {
    if (p2map.buckets[i].present && p2map.buckets[i].value > 1)
    {
      ++count2;
    }
  }
  PointMap_deinit(&p2map);

  printf("Part 1: %zu\n", count);
  printf("Part 2: %zu\n", count2);

  LineSegmentVec_deinit(&segs);
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
