#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include "output.h"

//does the output
bool
writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0 )
	return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}