#include "Advent/Advent.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static const struct option longOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
};

bool NS_(ParseCommandLine)(int argc, char** argv, CommandLineCommand* outCommand)
{
  bool result = false;
  int c;
  while ((c = getopt_long(argc, argv, "hV", longOptions, &argc)) != -1)
  {
    switch (c)
    {
    case 'h':
      result = true;
      *outCommand = COMMAND_LINE_COMMAND_PRINT_HELP;
      break;
    case 'V':
      result = true;
      *outCommand = COMMAND_LINE_COMMAND_PRINT_VERSION;
      break;
    default:
      result = true;
      *outCommand = COMMAND_LINE_COMMAND_PRINT_HELP_ERROR;
      break;
    }
  }
  return result;
}
