#pragma once

#include "AdventConfig.h"
#if __has_include("DayConfig.h")
#include "DayConfig.h"
#endif

#define ADVENT_INPUT_FILE_NAME ADVENT_DAY_NAME "Input.txt"

#define NS_(x) Advent_##x

char* NS_(ReadPuzzleInput)(const char* inputFileName);
