#include <errno.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "./options.h"


/* Check arguments.  */
int
options_processing(int argc, char **argv) {
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

  int c;
  while ((c = getopt(argc, argv, ":o:i:")) != -1) {
        switch(c) {
        case 'i':
            printf("case i");
            break;
        case 'o':
            printf("case o");
            break;
        case ':':       /* -f or -o without operand */
            fprintf(stderr,
                "Option -%c requires an operand\n", optopt);
            return 1;
            break;
        case '?':
            fprintf(stderr,
                "Unrecognized option: '-%c'\n", optopt);
            return 1;
        }
    }
  return nbytes;
}
  