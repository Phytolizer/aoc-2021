#pragma once

#include "AdventConfig.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define NS_(x) Advent_##x

#define COMMAND_LINE_COMMAND_VARIANTS_                                                                                 \
  X(NORMAL_OPERATION)                                                                                                  \
  X(PRINT_HELP)                                                                                                        \
  X(PRINT_HELP_ERROR)                                                                                                  \
  X(PRINT_VERSION)

typedef enum
{
#define X(x) COMMAND_LINE_COMMAND_##x,
  COMMAND_LINE_COMMAND_VARIANTS_
#undef X
} CommandLineCommand;

bool NS_(ParseCommandLine)(int argc, char** argv, CommandLineCommand* outCommand);
