
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"


double *filled_array(size_t length, double val) {
  double *res = (double *) malloc(length * sizeof(double));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

double randomd(const double min, const double max) {
  double x = (double) rand() / RAND_MAX;
  return min + x * (max - min);
}

double random1() {
  return (double) rand() / RAND_MAX;
}

void print_solution(size_t dim, const double *const solution) {
  for (size_t idx = 0; idx < dim; idx++) {
    printf("%08.4f\t", solution[idx]);
  }
  printf("\n");
}


/*******************************************************************************
  UNIT TESTS
******************************************************************************/

// TODO: Put up tests.
/*
Test(utils_unit, parse_args_full) {
    char* argv_[] = {"bench", "-v", "-a", "hgwosca", "-n", "20"};
    int argc_ = 6;
    Config args;
    parse_args(&args, argc_, argv_);
    cr_assert(args.verbose, "verbose flag should be set");
    cr_assert(strcmp(args.algorithm, "hgwosca") == 0, "algorithm should be 'hgwosca'");
    cr_assert(args.n_iterations == 20, "n should be set to 20");
}
*/