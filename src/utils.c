
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

double random_min_max(const double min, const double max) {
  double x = (double) rand() / RAND_MAX;
  return min + x * (max - min);
}

double random_0_to_1() {
  return (double) rand() / RAND_MAX;
}

void print_solution(size_t dim, const double *const solution) {
  for (size_t idx = 0; idx < dim; idx++) {
    printf("%.4f, ", solution[idx]);
  }
  printf("\n");
}
