
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


/**
   Prints the population to standard output.
 */
void print_population(size_t colony_size, size_t dim, const double *population) {
  for (size_t idx = 0; idx < colony_size; idx++) {
    printf("member%03ld, ", idx);
    print_solution(dim, &population[idx * dim]);
  }
}


double average_value(size_t arr_length, const double * arr) {
  double sum = 0;
  for (size_t idx = 0; idx < arr_length; ++idx) {
    sum += arr[idx];
  }
  return sum / arr_length;
}