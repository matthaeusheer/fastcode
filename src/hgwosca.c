/**
   Implementation of Grey Wolf Optimiser with Sine Cosine Algorithm for
   exploration.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hgwosca.h"
#include "utils.h"

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)(-1))
#endif


/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
double *gwo_init_population(size_t wolf_count,
                            size_t dim,
                            const double min_position,
                            const double max_position) {
  double *population = (double *) malloc(wolf_count * dim * sizeof(double));
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    for (size_t dimension = 0; dimension < dim; dimension++) {
      size_t idx = wolf * dim + dimension;
      population[idx] = random_min_max(min_position, max_position);
    }
  }
  return population;
}


/**
   Update the fitness of all wolves.
 */
void gwo_update_fitness(size_t wolf_count,
                        size_t dim,
                        obj_func_t obj_func,
                        double *const population, double *const fitness) {
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    fitness[wolf] = (*obj_func)(&population[wolf * dim], dim);
  }
}


/**
   Find the new wolf leaders.
 */
void gwo_update_leaders(size_t wolf_count,
                        double *const fitness,
                        size_t *const alpha,
                        size_t *const beta,
                        size_t *const delta) {
  *alpha = SIZE_MAX;
  *beta = SIZE_MAX;
  *delta = SIZE_MAX;
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    if (fitness[wolf] <= fitness[*alpha] || *alpha == SIZE_MAX) {
      *delta = *beta;
      *beta = *alpha;
      *alpha = wolf;
    } else if (fitness[wolf] <= fitness[*beta] || *beta == SIZE_MAX) {
      *delta = *beta;
      *beta = wolf;
    } else if (fitness[wolf] <= fitness[*delta] || *delta == SIZE_MAX) {
      *delta = wolf;
    }
  }
}


/**
   Initialise the fitness for all wolves.
 */
double *gwo_init_fitness(size_t wolf_count,
                         size_t dim,
                         obj_func_t obj_func,
                         double *const population) {
  double *const fitness = (double *const) malloc(wolf_count * sizeof(double));
  gwo_update_fitness(wolf_count, dim, obj_func, population, fitness);
  return fitness;
}


/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
double gwo_get_wolf_pos_update_dim_leader(size_t dimension,
                                          double a,
                                          const double *const wolf,
                                          const double *const leader_pos) {
  double r1 = random_0_to_1();
  double r2 = random_0_to_1();
  double A = 2 * a * r1 - a;                // see equation 3.3
  double C = 2 * r2;                        // see equation 3.4
  double D = fabs(C * leader_pos[dimension] - wolf[dimension]);     // see equation 3.1
  return leader_pos[dimension] - A * D;     // see equation 3.2
}


/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
double gwo_get_wolf_pos_update_dim_alpha(size_t dimension,
                                         double a,
                                         const double *const wolf,
                                         const double *const alpha_pos) {
  double r1 = random_0_to_1();
  double r2 = random_0_to_1();
  double A = 2 * a * r1 - a;                // see equation 3.3
  double C = 2 * r2;                        // see equation 3.4
  double D;
  if (random_0_to_1() < 0.5) {
    D = random_0_to_1() * sin(random_0_to_1()) * fabs(C * alpha_pos[dimension] - wolf[dimension]);
  } else {
    D = random_0_to_1() * cos(random_0_to_1()) * fabs(C * alpha_pos[dimension] - wolf[dimension]);
  }
  return alpha_pos[dimension] - A * D;
}


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
void gwo_update_wolf_position(size_t dim,
                              double a,
                              double *const wolf,
                              const double *const alpha_pos,
                              const double *const beta_pos,
                              const double *const delta_pos) {
  for (size_t idx = 0; idx < dim; idx++) {
    double new_pos = 0.0;
    new_pos += gwo_get_wolf_pos_update_dim_alpha(idx, a, wolf, alpha_pos);
    new_pos += gwo_get_wolf_pos_update_dim_leader(idx, a, wolf, beta_pos);
    new_pos += gwo_get_wolf_pos_update_dim_leader(idx, a, wolf, delta_pos);
    wolf[idx] = new_pos / 3;
  }
}


/**
   Update the positions of all wolves.
   TODO: test
 */
void gwo_update_all_positions(size_t wolf_count,
                              size_t dim,
                              double a,
                              double *const population,
                              size_t alpha, size_t beta, size_t delta) {
  const double *const alpha_pos = &population[alpha * dim];
  const double *const beta_pos = &population[beta * dim];
  const double *const delta_pos = &population[delta * dim];
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    if (wolf == alpha || wolf == beta || wolf == delta) {
      continue;
    }
    gwo_update_wolf_position(dim, a, &population[wolf * dim], alpha_pos, beta_pos, delta_pos);
  }
}


/**
   Clamp `val` between `min` and `max`.
 */
double gwo_clamp(double val, double min, double max) {
  return val < min ? min : (val > max ? max : val);
}


/**
   Clamp solutions into feasible space.
 */
void gwo_clamp_all_positions(size_t wolf_count, size_t dim,
                             double *const population,
                             const double min_position,
                             const double max_position) {
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    for (size_t dimension = 0; dimension < dim; dimension++) {
      population[wolf * dim + dimension] = gwo_clamp(population[wolf * dim + dimension],
                                                     min_position,
                                                     max_position);
    }
  }
}

size_t gwo_get_fittest_idx(size_t colony_size, const double *const fitness) {
  double max = -INFINITY;
  size_t max_idx = 0;
  for (size_t idx = 0; idx < colony_size; idx++) {
    if (fitness[idx] < max) {
      max = fitness[idx];
      max_idx = idx;
    }
  }
  return max_idx;
}


/**
   Run the Hybrid Grey Wolf Optimiser with Sine Cosine Algorithm.

   Args:
     - obj_func: the objective function to minimise.
     - wolf_count: the number of wolves to use.
     - max_iterations: the maximum number of iterations to perform.
     - min_positions: the minimum allowed position for each dimension.
     - max_positions: the maximum allowed position for each dimension.

   Returns:
     The position minimising the objective function. This array has size `dim`.
 */
double *gwo_hgwosca(obj_func_t obj_func,
                    size_t wolf_count,
                    size_t dim,
                    size_t max_iterations,
                    const double min_position,
                    const double max_position) {
  srand(100);

  double *const population = gwo_init_population(wolf_count, dim, min_position, max_position);
  double *const fitness = gwo_init_fitness(wolf_count, dim, obj_func, population);
  size_t alpha = 0, beta = 0, delta = 0;

  #ifdef DEBUG
    print_population(wolf_count, dim, population);  // Print the initialized population
    printf("# AVG FITNESS: %f\n", average_value(wolf_count, fitness));
  #endif

  for (size_t iter = 0; iter < max_iterations; iter++) {
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    double a = 2 - iter * ((double) 2 / max_iterations);
    gwo_update_all_positions(wolf_count, dim, a, population, alpha, beta, delta);
    gwo_clamp_all_positions(wolf_count, dim, population, min_position, max_position);
    gwo_update_fitness(wolf_count, dim, obj_func, population, fitness);

    #ifdef DEBUG
        print_population(wolf_count, dim, population);
        printf("# AVG FITNESS: %f\n", average_value(wolf_count, fitness));
        // printf("\nBEST SOLUTION: %ld\n", best_solution);
        // print_solution(dim, &fitness);
    #endif
  }

  gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  double *const best_solution = (double *const) malloc(dim * sizeof(double));
  memcpy(best_solution, &population[alpha * dim], dim * sizeof(double));

  free(population);
  free(fitness);

  return best_solution;
}
