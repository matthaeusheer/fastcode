#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "utils.h"

float * gwo_hgwosca(obj_func_t obj_func,
                     size_t wolf_count,
                     size_t dim,
                     size_t max_iterations,
                     float min_position,
                     float max_position);

/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
void gwo_init_population(float *population,
                         size_t wolf_count, size_t dim,
                         float min_position, float max_position);

/**
   Update the fitness of all wolves.
 */
void gwo_update_fitness(size_t wolf_count,
                        size_t dim,
                        obj_func_t obj_func,
                        float *population,
                        float *fitness);

/**
   Find the new wolf leaders.
 */
void gwo_update_leaders(size_t wolf_count, float *fitness, size_t *alpha, size_t *beta, size_t *delta);

/**
   Initialise the fitness for all wolves.
 */
void gwo_init_fitness(float *fitness, size_t wolf_count,
                      size_t dim, obj_func_t obj_func, float *population);

/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
float gwo_get_wolf_pos_update_dim_leader(size_t dimension, float a, const float *wolf, const float *leader_pos);

/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
float gwo_get_wolf_pos_update_dim_alpha(size_t dimension, float a, const float *wolf, const float *alpha_pos);


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
void gwo_update_wolf_position(size_t dim,
                              float a,
                              float *wolf,
                              const float *alpha_pos,
                              const float *beta_pos,
                              const float *delta_pos);

/**
   Update the positions of all wolves.
   TODO: test
 */
void gwo_update_all_positions(size_t wolf_count,
                              size_t dim,
                              float a,
                              float *population,
                              size_t alpha, size_t beta, size_t delta);


/**
   Clamp `val` between `min` and `max`.
 */
float gwo_clamp(float val, float min, float max);

/**
   Clamp solutions into feasible space.
 */
void gwo_clamp_all_positions(size_t wolf_count, size_t dim, float *population,
                             float min_position, float max_position);

size_t gwo_get_fittest_idx(size_t colony_size, const float *fitness);

#ifdef __cplusplus
}
#endif // __cplusplus
