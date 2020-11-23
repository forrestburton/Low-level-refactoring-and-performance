#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "output.h"

//does the output
bool
writebytes (unsigned long long x, int nbytes, int block_size)
{
  if (block_size == -1)
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
  else
  {
    int buffer_size = nbytes;
    //fprintf (stderr, "%d \n", buffer_size);

    char *buffer = (char*) malloc(buffer_size); //allocate buffer of size block_size KB
    memcpy(buffer, &x, buffer_size); //copy part of x to buffer
    write(1, buffer, buffer_size); //print out buffer in blocks of size 
    
    free(buffer);
    return true; 
  }
}