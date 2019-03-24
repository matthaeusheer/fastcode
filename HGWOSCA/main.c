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

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)(-1))
#endif


/**
   Generate a random double between min and max.
 */
static double randomd(const double min, const double max) {
  double x = (double)rand() / RAND_MAX;
  return min + x * (max - min);
}


/**
   Generate a random double 0 and 1.
 */
static double random1() {
  return (double)rand() / RAND_MAX;
}


/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
static double* const init_population(size_t wolf_count, size_t dim,
                                     const double* const min_positions,
                                     const double* const max_positions) {
  double* population = (double*)malloc(wolf_count * dim * sizeof(double));
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    for(size_t dimension = 0; dimension < dim; dimension++) {
      size_t idx = wolf * dim + dimension;
      population[idx] = randomd(min_positions[dimension], max_positions[dimension]);
    }
  }
  return population;
}


/**
   Update the fitness of all wolves.
 */
static void update_fitness(size_t wolf_count, size_t dim,
                           double(*obj)(const double* const, size_t),
                           double* const population, double* const fitness) {
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    fitness[wolf] = (*obj)(&population[wolf * dim], dim);
  }
}


/**
   Find the new wolf leaders.
 */
static void update_leaders(size_t wolf_count, double* const fitness,
                           size_t* const alpha,
                           size_t* const beta,
                           size_t* const delta) {
  *alpha = SIZE_MAX;
  *beta = SIZE_MAX;
  *delta = SIZE_MAX;
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    if(fitness[wolf] <= fitness[*alpha] || *alpha == SIZE_MAX) {
      *delta = *beta;
      *beta = *alpha;
      *alpha = wolf;
    } else if(fitness[wolf] <= fitness[*beta] || *beta == SIZE_MAX) {
      *delta = *beta;
      *beta = wolf;
    } else if(fitness[wolf] <= fitness[*delta] || *delta == SIZE_MAX) {
      *delta = wolf;
    }
  }
}


/**
   Initialise the fitness for all wolves.
 */
static double* const init_fitness(size_t wolf_count, size_t dim,
                                  double(*obj)(const double* const, size_t),
                                  double* const population) {
  double* const fitness = (double* const)malloc(wolf_count * sizeof(double));
  update_fitness(wolf_count, dim, obj, population, fitness);
  return fitness;
}


/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
static double get_wolf_pos_update_dim_leader(size_t dimension, double a,
                                             const double* const wolf,
                                             const double* const leader_pos) {
  double r1 = random1();
  double r2 = random1();
  double A = 2 * a * r1 - a;
  double C = 2 * r2;
  double D = (C * leader_pos[dimension] - wolf[dimension]);
  return leader_pos[dimension] - A * D;
}


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
static void update_wolf_position(size_t dim, double a, double* const wolf,
                                 const double* const alpha_pos,
                                 const double* const beta_pos,
                                 const double* const delta_pos) {
  for(size_t idx = 0; idx < dim; idx++) {
    double new_pos = 0.0;
    new_pos += get_wolf_pos_update_dim_leader(idx, a, wolf, alpha_pos);
    new_pos += get_wolf_pos_update_dim_leader(idx, a, wolf, beta_pos);
    new_pos += get_wolf_pos_update_dim_leader(idx, a, wolf, delta_pos);
    wolf[idx] = new_pos / 3;
  }
}


/**
   Update the positions of all wolves.
   TODO: test
 */
static void update_all_positions(size_t wolf_count, size_t dim, double a,
                                 double* const population,
                                 size_t alpha, size_t beta, size_t delta) {
  const double* const alpha_pos = &population[alpha * dim];
  const double* const beta_pos = &population[beta * dim];
  const double* const delta_pos = &population[delta * dim];
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    update_wolf_position(dim, a, &population[wolf * dim], alpha_pos, beta_pos, delta_pos);
  }
}


/**
   Clamp `val` between `min` and `max`.
 */
static double clamp(double val, double min, double max) {
  return val < min ? min : (val > max ? max : val);
}


/**
   Clamp solutions into feasible space.
 */
static void clamp_all_positions(size_t wolf_count, size_t dim,
                                double* const population,
                                const double* const min_positions,
                                const double* const max_positions) {
  for(size_t wolf = 0; wolf < wolf_count; wolf++) {
    for(size_t dimension = 0; dimension < dim; dimension++) {
      population[wolf * dim + dimension] = clamp(population[wolf * dim + dimension],
                                                 min_positions[dimension],
                                                 max_positions[dimension]);
    }
  }
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
double* const hgwosca(double(*obj)(const double* const, size_t),
                            size_t wolf_count,
                            size_t dim,
                            size_t max_iterations,
                            const double* const min_positions,
                            const double* const max_positions) {
  double* const population = init_population(wolf_count, dim, min_positions, max_positions);
  double* const fitness = init_fitness(wolf_count, dim, obj, population);
  size_t alpha = 0, beta = 0, delta = 0;

  for(size_t iter = 0; iter < max_iterations; iter++) {
    update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    double a = 2 - iter * ((double)2 / max_iterations);
    update_all_positions(wolf_count, dim, a, population, alpha, beta, delta);
    clamp_all_positions(wolf_count, dim, population, min_positions, max_positions);
    update_fitness(wolf_count, dim, obj, population, fitness);
  }

  update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  double* const best_solution = (double* const)malloc(dim * sizeof(double));
  memcpy(best_solution, &population[alpha * dim], dim * sizeof(double));

  free(population);
  free(fitness);

  return best_solution;
}



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


Test(hgwosca_unit, random1) {
  srand((unsigned) time(NULL));

  for(size_t iter = 0; iter < 20; iter++) {
    double r = random1();
    cr_assert(r <= 1, "random1 should be bound above by 1");
    cr_assert(r >= 0, "random1 should be bound below by 0");
  }
}


Test(hgwosca_unit, init_population) {
  size_t wolf_count = 40;
  size_t dim = 4;
  double mins[] = {0.0, 4.0, 0.001, 0.3};
  double maxs[] = {10.0, 4.0, 0.002, 0.6};
  double* const population = init_population(wolf_count, dim, mins, maxs);
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


static double sum_of_squares(const double* const args, size_t dim) {
  double sum = 0.0;
  for(size_t idx = 0; idx < dim; idx++) {
    sum += pow(args[idx], 2.0);
  }
  return sum;
}

Test(hgwosca_unit, update_fitness) {
  size_t wolf_count = 4;
  size_t dim = 2;
  double population[] = {
    0.0, 0.0,
    1.0, 4.0,
    5.0, 0.0,
    2.0, 3.0
  };
  double fitness[] = {0.0, 0.0, 0.0, 0.0};
  update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
  cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
  cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
  cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
  cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, init_fitness) {
  size_t wolf_count = 4;
  size_t dim = 2;
  double population[] = {
    0.0, 0.0,
    1.0, 4.0,
    5.0, 0.0,
    2.0, 3.0
  };
  double* const fitness = init_fitness(wolf_count, dim, sum_of_squares, population);
  update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
  cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
  cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
  cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
  cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, update_leaders) {
  size_t wolf_count = 10;
  size_t alpha = 0, beta = 0, delta = 0;
  double fitness[] = {1.0, 2.0, 3.0, 3.0, 0.0, 5.0, 2.0, 1.0, 0.0, -1.0};
  update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  cr_assert(alpha == 9, "alpha should be 10th wolf");
  cr_assert((beta == 4 && delta == 8) || (delta == 4 && beta == 8), "beta and delta should "
            "be wolves 5 and 9, unordered");
  fitness[8] = 8.0;
  fitness[4] = -2.0;
  fitness[7] = 5.5;
  /* double fitness[] = {1.0, 2.0, 3.0, 3.0, -2.0, 5.0, 2.0, 5.5, 8.0, -1.0}; */
  update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  cr_assert(alpha == 4, "alpha should be 5th wolf");
  cr_assert(beta == 9, "beta should be 10th wolf");
  cr_assert(delta == 0, "delta should be 1st wolf");
}


Test(hgwosca_unit, clamp) {
  double vals[] = { -10.0, 0.0, 400, -3.0, 5.01};
  double mins[] = { 5.0, -5.0, 30, -10, 0};
  double maxs[] = { 100, 1.0, 30, -4, 5};
  for(size_t idx = 0; idx < 5; idx++) {
    double res = clamp(vals[idx], mins[idx], maxs[idx]);
    cr_assert(res <= maxs[idx], "clamped value is bound above by max");
    cr_assert(res >= mins[idx], "clamped value is bound below by min");
  }
}


Test(hgwosca_unit, clamp_all_positions) {
  double vals[] = {
    -10.0, 0.0, 400, -3.0, 5.01,
    45, 3, 30, -4, 5,
    -100, -100, -100, -100, 100
  };
  double mins[] = { 5.0, -5.0, 30, -10, 0};
  double maxs[] = { 100, 1.0, 30, -4, 5};
  clamp_all_positions(3, 5, vals, mins, maxs);
  for(size_t idx = 0; idx < 5; idx++) {
    cr_assert(vals[idx] <= maxs[idx], "clamped value is bound above by max");
    cr_assert(vals[idx] >= mins[idx], "clamped value is bound below by min");
  }
}
