/**************
BASIC SQUIRREL INTEGRATION TEST
***************/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "objectives.h"
#include "squirrel.h"
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define POPULATION 10000
#define DIM 2

Test(squirrel_integration, sum_of_squares) {
  size_t population = POPULATION;
  size_t dim = DIM;
  size_t max_iter = 250;
  double* mins = filled_double_array(dim, -10);
  double* maxs = filled_double_array(dim, 10);

  double* solution = squirrel(sum_of_squares, population, dim, max_iter, mins, maxs);
  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "sos objective should be minimised at 0");

  free(solution);
  free(mins);
  free(maxs);
}


Test(squirrel_integration, rosenbrock) {
  size_t population = POPULATION;
  size_t dim = DIM;
  size_t max_iter = 1000;
  double* mins = filled_double_array(dim, -30);
  double* maxs = filled_double_array(dim, 30);
  double* solution = squirrel(rosenbrock, population, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close "
              "to 1");
  }
  free(solution);
  free(mins);
  free(maxs);
}

Test(squirrel_integration, sphere) {
  size_t population = POPULATION;
  size_t dim = DIM;
  size_t max_iter = 500;
  double* mins = filled_double_array(dim, -5);
  double* maxs = filled_double_array(dim, 5);
  double* solution = squirrel(sphere, population, dim, max_iter, mins, maxs);

  cr_assert(fabs(sphere(solution, dim) - 0 ) < 0.1, "sphere objective should be minimised at 0");

  free(solution);
  free(mins);
  free(maxs);
}
