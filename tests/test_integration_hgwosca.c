/**
   Integration tests for numerical correctness of algorithms.
*/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "objectives.h"
#include "hgwosca.h"
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


// TODO: (jakob) Quite some code duplication. Maybe setup and teardown actions and common code can be collected.

Test(hgwosca_integration, sum_of_squares) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 120;
  double* mins = filled_double_array(dim, -100);
  double* maxs = filled_double_array(dim, 100);
  double* solution = gwo_hgwosca(sum_of_squares, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.00001, "each dimension should be zero");
  }
  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "objective should be minimised at 0");
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, sum) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 100;
  double* mins = filled_double_array(dim, -100);
  double* maxs = filled_double_array(dim, 100);
  double* solution = gwo_hgwosca(sum, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] + 100) < 0.00001, "each dimension should be -100");
  }
  cr_assert(fabs(sum(solution, dim) + 100 * dim) < 0.2 * dim, "objective should be "
            "minimised at -100*dim");
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, sum_2) {
  size_t wolf_count = 50;
  size_t dim = 2;
  size_t max_iter = 100;
  double* mins = filled_double_array(dim, -100);
  double* maxs = filled_double_array(dim, 100);
  double* solution = gwo_hgwosca(sum, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] + 100) < 0.00001, "each dimension should be -100");
  }
  cr_assert(fabs(sum(solution, dim) + 100 * dim) < 0.2 * dim, "objective should be "
                                                              "minimised at -100*dim");
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, rosenbrock) {
  size_t wolf_count = 40;
  size_t dim = 3;
  size_t max_iter = 1500;
  double* mins = filled_double_array(dim, -100);
  double* maxs = filled_double_array(dim, 100);
  double* solution = gwo_hgwosca(rosenbrock, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close "
              "to 1");
  }
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, sphere) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 800;
  double* mins = filled_double_array(dim, -100);
  double* maxs = filled_double_array(dim, 100);
  double* solution = gwo_hgwosca(sphere, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
      cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close "
      "to 0");
  }
  cr_assert(fabs(sphere(solution, dim)) < 0.5, "objective should be minimised at 0");
  free(solution);
  free(mins);
  free(maxs);
}
