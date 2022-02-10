#pragma once

#include <stddef.h>

typedef struct
{
  char* data;
  size_t length;
} AdventString;

typedef struct
{
  const char* data;
  size_t length;
} AdventSView;
