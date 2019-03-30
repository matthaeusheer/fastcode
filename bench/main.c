#ifndef WIN32
#include <sys/time.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ops.h"
#include "utils.h"


int main(int argc, char* argv[]) {
  Clargs args;
  parse_args(&args, argc, argv);

  if(args.verbose) {
    printf("Algorithm=%s / n=%i", args.algorithm, args.n);
  } else {
    operation_benchmark();
  }
}
