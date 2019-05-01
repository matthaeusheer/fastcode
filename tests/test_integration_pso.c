/**************
BASIC PSO INTEGRATION TEST
***************/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "objectives.h"
#include "pso.h"
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define SWARM_SIZE 1000
#define DIM 2

Test(pso_basic_integration, sum_of_squares) {
  size_t swarm_size = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 1000;
  double* mins = filled_double_array(dim, -10);
  double* maxs = filled_double_array(dim, 10);
  double* solution = pso_basic(sum_of_squares, swarm_size, dim, max_iter, mins, maxs);

  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "sos objective should be minimised at 0");

  free(solution);
  free(mins);
  free(maxs);
}

Test(pso_basic_integration, sphere) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 1000;
  double* mins = filled_double_array(dim, -5);
  double* maxs = filled_double_array(dim, 5);
  double* solution = pso_basic(sphere, swarm_count, dim, max_iter, mins, maxs);

  cr_assert(fabs(sphere(solution, dim) - 0 ) < 0.1, "sphere objective should be minimised at 0");

  free(solution);
  free(mins);
  free(maxs);
}

Test(pso_basic_integration, rastigrin) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 1000;
  double* mins = filled_double_array(dim, -5);
  double* maxs = filled_double_array(dim, 5);
  double* solution = pso_basic(rastigrin, swarm_count, dim, max_iter, mins, maxs);

  cr_assert(fabs(rastigrin(solution, dim) - 0 ) < 0.1, "rastigrin objective should be minimised at 0");

  free(solution);
  free(mins);
  free(maxs);
}


Test(pso_integration, rosenbrock) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 1000;
  double* mins = filled_double_array(dim, -30);
  double* maxs = filled_double_array(dim, 30);
  double* solution = pso_basic(rosenbrock, swarm_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close "
              "to 1");
  }
  free(solution);
  free(mins);
  free(maxs);
}
