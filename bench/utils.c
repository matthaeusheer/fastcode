#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include "criterion/criterion.h"

#include "utils.h"

#define ARRAY_SIZE 1e7
#define USAGE (                                                         \
               "Usage: %s [-van]\n"                                     \
               "  -v    verbose\n"                                      \
               "  -a    algorithm name\n"                               \
               "  -n    number of iterations to run (default=2048)\n"   \
               "\n"                                                     \
               "  If no algorithm name is given, the operation\n"       \
               "  benchmark is performed.\n"                            \
               "\n"                                                     \
               "  Example:\n"                                           \
               "    %s -a hgwosca -n 4048\n"                            \
               "    will run the HGWOSCA algorithm for 4048\n"          \
               "    iterations. No verbose output will be provided.\n")

/**
   Generate a random number between 0 and 1.
*/
double randf() {
  return (double)rand() / (double)RAND_MAX;
}


/**
   Build an array of size `ARRAY_SIZE` and fill it with random numbers.
*/
double* build() {
  double* x = (double*)malloc(ARRAY_SIZE * sizeof(double));
  for(size_t i = 0; i < ARRAY_SIZE; ++i) {
    x[i] = randf();
  }
  return x;
}


/**
   Parse command line arguments.
*/
void parse_args(Clargs* args, int argc, char* argv[]) {
  int opt;
  int n = 2048;
  args->verbose = false;
  args->algorithm = NULL;
  args->n = n;
  while((opt = getopt(argc, argv, "hva:n:")) != -1) {
    switch (opt) {
    case 'v':
      args->verbose = true;
      break;
    case 'a':
      args->algorithm = optarg;
      break;
    case 'n':
      if(sscanf(optarg, "%i", &n) != 1) {
        fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
        exit(EXIT_FAILURE);
      }
      args->n = n;
      break;
    case 'h':
    default:
      fprintf(stderr, USAGE, argv[0], argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}


/*******************************************************************************
  UNIT TESTS
******************************************************************************/

Test(utils_unit, parse_args_full) {
  char* argv_[] = {"bench", "-v", "-a", "hgwosca", "-n", "20"};
  int argc_ = 6;
  Clargs args;
  parse_args(&args, argc_, argv_);
  cr_assert(args.verbose, "verbose flag should be set");
  cr_assert(strcmp(args.algorithm, "hgwosca") == 0, "algorithm should be 'hgwosca'");
  cr_assert(args.n == 20, "n should be set to 20");
}


Test(utils_unit, parse_args_partial) {
  char* argv_[] = {"bench"};
  int argc_ = 1;
  Clargs args;
  parse_args(&args, argc_, argv_);
  cr_assert(!args.verbose, "verbose flag should not be set");
  cr_assert(args.algorithm == NULL, "algorithm should be null");
  cr_assert(args.n == 2048, "n should be set to 2048");
}
