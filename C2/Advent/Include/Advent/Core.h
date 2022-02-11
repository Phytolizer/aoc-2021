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

#define ADVENT_STRING_AS_VIEW(S)                                                                                       \
  (AdventSView)                                                                                                        \
  {                                                                                                                    \
    (S).data, (S).length                                                                                               \
  }

#define ADVENT_SVIEW_C(C)                                                                                              \
  (AdventSView)                                                                                                        \
  {                                                                                                                    \
    C, sizeof(C) - 1                                                                                                   \
  }

typedef struct
{
  AdventSView first;
  AdventSView second;
} AdventSViewPair;

AdventSViewPair adventSViewTokenize(AdventSView input, AdventSView breaks);
