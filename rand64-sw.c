#include <immintrin.h>
#include <stdio.h>
#include "rand64-sw.h"

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/random", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}



/* Initialize the software file implementation.  */
void
software_file_init (void)
{
  urandstream = fopen (rsrc, "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
unsigned long long
software_file (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
void
software_file_fini (void)
{
  fclose (urandstream);
}