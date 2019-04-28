#include <criterion/criterion.h>

#include <math.h>
#include "objectives.h"
#include "penguin.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>


Test(pengu_integration, sum_of_squares) {
  size_t population_size = 50;
  size_t dim = 2;
  size_t max_iter = 100;
  double* mins = filled_double_array(dim, -5);
  double* maxs = filled_double_array(dim, 5);
  double* solution = pen_emperor_penguin(sum_of_squares, population_size, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be zero");
  }
  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "objective should be minimised at 0");
  free(solution);
  free(mins);
  free(maxs);
}


Test(pengu_integration, rosenbrock) {
  size_t population_size = 50;
  size_t dim = 2;
  size_t max_iter = 100;
  double* mins = filled_double_array(dim, -5);
  double* maxs = filled_double_array(dim, 5);
  double* solution = pen_emperor_penguin(rosenbrock, population_size, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) - 1.0 < 0.1, "each dimension should be one");
  }
  cr_assert(fabs(sum_of_squares(solution, dim)) < 3, "objective should be minimised at 0");
  free(solution);
  free(mins);
  free(maxs);
}
