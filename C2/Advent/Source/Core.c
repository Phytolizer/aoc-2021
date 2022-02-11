#include "Advent/Core.h"
#include <string.h>

size_t sviewSpan(AdventSView input, AdventSView breaks)
{
  size_t cursor = 0;
  while (cursor < input.length && memchr(breaks.data, input.data[cursor], breaks.length) != NULL)
  {
    cursor += 1;
  }
  return cursor;
}

AdventSViewPair adventSViewTokenize(AdventSView input, AdventSView breaks)
{
  size_t cursor = 0;
  while (cursor < input.length && memchr(breaks.data, input.data[cursor], breaks.length) == NULL)
  {
    cursor += 1;
  }
  AdventSView first = {input.data, cursor};
  if (cursor == input.length)
  {
    return (AdventSViewPair){first, {0}};
  }
  AdventSView second = {input.data + cursor, input.length - cursor};
  size_t span = sviewSpan(second, breaks);
  second.data += span;
  second.length -= span;
  return (AdventSViewPair){first, second};
}
