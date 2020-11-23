#include <immintrin.h>
#include <stdio.h>
#include <time.h>    // time()
#include "rand64-hw.h"


/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
_Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}

/* Initialize the hardware rand64 implementation.  */
struct drand48_data buf = {0};
long int a, b;

void
hardware_rand64_init (void)
{
}

/* Return a random value, using hardware operations.  */
unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
void
hardware_rand64_fini (void)
{
}



//MRAND_48 option initialization
void
mrand48_rng_init (void)
{
  srand48_r(time(NULL), &buf);
}

/* Return a random value, using mrand48 hardware operations.  */
unsigned long long
mrand48_rng (void)
{
  mrand48_r(&buf, &a);
  mrand48_r(&buf, &b);

  unsigned long long int x = (((unsigned long long) a) << 32) | ((unsigned long long) b & 0x00000000FFFFFFFF);
  return x;
}

/* Finalize the hardware mrand48 implementation.  */
void
mrand48_rng_fini (void)
{
}



