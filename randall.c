/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./output.h"
#include "./options.h"
#include "./rand64-hw.h"
#include "./rand64-sw.h"

/* Hardware implementation.  


struct cpuid { unsigned eax, ebx, ecx, edx; };


static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}


static _Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}


static void
hardware_rand64_init (void)
{
}


static unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}


static void
hardware_rand64_fini (void)
{
}



 Software implementation.  

static FILE *urandstream;

static void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/random", "r");
  if (! urandstream)
    abort ();
}


static unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

static void
software_rand64_fini (void)
{
  fclose (urandstream);
}


static bool
writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
	return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}
*/

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  //dynamically allocate opts struct
  struct opts opts;
  options_processing(argc, argv, &opts);

  if (opts.valid == false)
  {
    fprintf (stderr, "%s: usage: %s invalid option\n", argv[0], argv[0]);
    return 1;
  }
 
  /* Check arguments. 
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }
  */

  /* If there's no work to do, don't worry about which library to use.  */
  if (opts.nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if (opts.input == MRAND48_R) 
  {
    initialize = mrand48_rng_init;
    rand64 = mrand48_rng;
    finalize = mrand48_rng_fini;
  }
  else if (opts.input == SLASH_F)  //use file path instead of /dev/random in sofware impl
  {
    rsrc = opts.r_src;
    initialize = software_file_init; 
    rand64 = software_file;
    finalize = software_file_fini;
  }
  else 
  {
    if (rdrand_supported ())
      {
        initialize = hardware_rand64_init;
        rand64 = hardware_rand64;
        finalize = hardware_rand64_fini;
      }
    else
      {
        initialize = software_rand64_init;
        rand64 = software_rand64;
        finalize = software_rand64_fini;
      }
  }

  initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      int outbytes;
      unsigned long long x = rand64 ();
      if (opts.block_size == -1) {
        outbytes = opts.nbytes < wordsize ? opts.nbytes : wordsize;
      }
      else {
        outbytes = opts.block_size*1024;
        if (opts.nbytes < outbytes) { //account for remainder
          outbytes = opts.nbytes;
        }
      }
      if (!writebytes (x, outbytes, opts.block_size))
	{
	  output_errno = errno;
	  break;
	}
      opts.nbytes -= outbytes;
    }
  while (0 < opts.nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
