/**
   Implementation of Grey Wolf Optimiser with Sine Cosine Algorithm for
   exploration.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <float.h>

#include <criterion/criterion.h>

#include "main.h"


/**
   Generate a random double between min and max.
 */
static double randomd(const double min, const double max) {
  double x = (double)rand() / RAND_MAX;
  return min + x * (max - min);
}


/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
static double* init_population(size_t wolf_count, size_t dim,
                               const double* const min_positions,
                               const double* const max_positions) {
  double* population = (double*)malloc(wolf_count * sizeof(double));
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    for(size_t dimension = 0; dimension < dim; dimension++) {
      size_t idx = wolf * dim + dimension;
      population[idx] = randomd(min_positions[dimension], max_positions[dimension]);
    }
  }
  return population;
}


/**
   Run the Hybrid Grey Wolf Optimiser with Sine Cosine Algorithm.

   Args:
     - obj: the objective function to minimise.
     - wolf_count: the number of wolves to use.
     - max_iterations: the maximum number of iterations to perform.
     - min_positions: the minimum allowed position for each dimension.
     - max_positions: the maximum allowed position for each dimension.

   Returns:
     The position minimising the objective function. This array has size `dim`.
 */
double* hgwosca(double(*obj)(const double* const, size_t),
                size_t wolf_count,
                size_t dim,
                size_t max_iterations,
                const double* const min_positions,
                const double* const max_positions);



/*******************************************************************************
  UNIT TESTS
******************************************************************************/


Test(hgwosca_unit, randomd) {
  srand((unsigned) time(NULL));

  double r = randomd(0.0, 1.0);
  cr_assert(r <= 1.0, "randomd upper bound 1");
  cr_assert(r >= 0.0, "randomd lower bound 1");

  r = randomd(-5.0, 0.0);
  cr_assert(r <= 0.0, "randomd upper bound 2");
  cr_assert(r >= -5.0, "randomd lower bound 2");

  r = randomd(-100.0, 100.0);
  cr_assert(r <= 100.0, "randomd upper bound 3");
  cr_assert(r >= -100.0, "randomd lower bound 3");

  r = randomd(0.0, 0.0);
  cr_assert(r <= 0.0, "randomd upper bound 4");
  cr_assert(r >= 0.0, "randomd lower bound 4");
}


Test(hgwosca_unit, init_population) {
  size_t wolf_count = 40;
  size_t dim = 4;
  double mins[] = {0.0, 4.0, 0.001, 0.3};
  double maxs[] = {10.0, 4.0, 0.002, 0.6};
  double* population = init_population(wolf_count, dim, mins, maxs);
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    cr_assert(population[wolf * dim] <= maxs[0], "first dimension should be bound above");
    cr_assert(population[wolf * dim] >= mins[0], "first dimension should be bound below");
    cr_assert(population[wolf * dim + 1] <= maxs[1], "second dimension should be bound above");
    cr_assert(population[wolf * dim + 1] >= mins[1], "second dimension should be bound below");
    cr_assert(population[wolf * dim + 2] <= maxs[2], "third dimension should be bound above");
    cr_assert(population[wolf * dim + 2] >= mins[2], "third dimension should be bound below");
    cr_assert(population[wolf * dim + 3] <= maxs[3], "fourth dimension should be bound above");
    cr_assert(population[wolf * dim + 3] >= mins[3], "fourth dimension should be bound below");
  }
}
