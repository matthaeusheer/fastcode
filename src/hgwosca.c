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
void gwo_init_population(float* const population,
                         size_t wolf_count,
                         size_t dim,
                         const float min_position,
                         const float max_position) {
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    for (size_t dimension = 0; dimension < dim; dimension++) {
      size_t idx = wolf * dim + dimension;
      population[idx] = random_min_max(min_position, max_position);
    }
  }
}


/**
   Update the fitness of all wolves.
 */
void gwo_update_fitness(size_t wolf_count,
                        size_t dim,
                        obj_func_t obj_func,
                        float *const population, float *const fitness) {
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    fitness[wolf] = (*obj_func)(&population[wolf * dim], dim);
  }
}


/**
   Find the new wolf leaders.
 */
void gwo_update_leaders(size_t wolf_count,
                        float *const fitness,
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
void gwo_init_fitness(float *const fitness,
                      size_t wolf_count,
                      size_t dim,
                      obj_func_t obj_func,
                      float *const population) {
  gwo_update_fitness(wolf_count, dim, obj_func, population, fitness);
}


/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
float gwo_get_wolf_pos_update_dim_leader(size_t dimension,
                                          float a,
                                          const float *const wolf,
                                          const float *const leader_pos) {
  float r1 = random_0_to_1();
  float r2 = random_0_to_1();
  float A = 2 * a * r1 - a;                // see equation 3.3
  float C = 2 * r2;                        // see equation 3.4
  float D = fabs(C * leader_pos[dimension] - wolf[dimension]);     // see equation 3.1
  return leader_pos[dimension] - A * D;     // see equation 3.2
}


/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
float gwo_get_wolf_pos_update_dim_alpha(size_t dimension,
                                         float a,
                                         const float *const wolf,
                                         const float *const alpha_pos) {
  float r1 = random_0_to_1();
  float r2 = random_0_to_1();
  float A = 2 * a * r1 - a;                // see equation 3.3
  float C = 2 * r2;                        // see equation 3.4
  float D;
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
                              float a,
                              float *const wolf,
                              const float *const alpha_pos,
                              const float *const beta_pos,
                              const float *const delta_pos) {
  for (size_t idx = 0; idx < dim; idx++) {
    float new_pos = 0.0;
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
                              float a,
                              float *const population,
                              size_t alpha, size_t beta, size_t delta) {
  const float *const alpha_pos = &population[alpha * dim];
  const float *const beta_pos = &population[beta * dim];
  const float *const delta_pos = &population[delta * dim];
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
float gwo_clamp(float val, float min, float max) {
  return val < min ? min : (val > max ? max : val);
}


/**
   Clamp solutions into feasible space.
 */
void gwo_clamp_all_positions(size_t wolf_count, size_t dim,
                             float *const population,
                             const float min_position,
                             const float max_position) {
  for (size_t wolf = 0; wolf < wolf_count; wolf++) {
    for (size_t dimension = 0; dimension < dim; dimension++) {
      population[wolf * dim + dimension] = gwo_clamp(population[wolf * dim + dimension],
                                                     min_position,
                                                     max_position);
    }
  }
}

size_t gwo_get_fittest_idx(size_t colony_size, const float *const fitness) {
  float max = -INFINITY;
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
float *gwo_hgwosca(obj_func_t obj_func,
                    size_t wolf_count,
                    size_t dim,
                    size_t max_iterations,
                    const float min_position,
                    const float max_position) {
  srand(100);

  float population[wolf_count * dim];
  float fitness[wolf_count];
  gwo_init_population(population, wolf_count, dim, min_position, max_position);
  gwo_init_fitness(fitness, wolf_count, dim, obj_func, population);
  size_t alpha = 0, beta = 0, delta = 0;

  #ifdef DEBUG
    print_population(wolf_count, dim, population);  // Print the initialized population
    printf("# AVG FITNESS: %f\n", average_value(wolf_count, fitness));
    printf("# BEST FITNESS: %f\n", lowest_value(wolf_count, fitness));
  #endif

  for (size_t iter = 0; iter < max_iterations; iter++) {
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    float a = 2 - iter * ((float) 2 / max_iterations);
    gwo_update_all_positions(wolf_count, dim, a, population, alpha, beta, delta);
    gwo_clamp_all_positions(wolf_count, dim, population, min_position, max_position);
    gwo_update_fitness(wolf_count, dim, obj_func, population, fitness);

    #ifdef DEBUG
      print_population(wolf_count, dim, population);
      printf("# AVG FITNESS: %f\n", average_value(wolf_count, fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(wolf_count, fitness));
    #endif
  }

  gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  float *const best_solution = (float *const) malloc(dim * sizeof(float));
  memcpy(best_solution, &population[alpha * dim], dim * sizeof(float));

  return best_solution;
}
