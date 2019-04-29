#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "utils.h"

double * gwo_hgwosca(obj_func_t obj_func,
                     size_t wolf_count,
                     size_t dim,
                     size_t max_iterations,
                     const double *min_positions,
                     const double *max_positions);

/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
double *gwo_init_population(size_t wolf_count, size_t dim, const double * min_positions, const double * max_positions);

/**
   Update the fitness of all wolves.
 */
void gwo_update_fitness(size_t wolf_count,
                        size_t dim,
                        obj_func_t obj_func,
                        double *population,
                        double *fitness);

/**
   Find the new wolf leaders.
 */
void gwo_update_leaders(size_t wolf_count, double *fitness, size_t *alpha, size_t *beta, size_t *delta);

/**
   Initialise the fitness for all wolves.
 */
double *gwo_init_fitness(size_t wolf_count, size_t dim, obj_func_t obj_func, double *population);

/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
double gwo_get_wolf_pos_update_dim_leader(size_t dimension, double a, const double *wolf, const double *leader_pos);

/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
double gwo_get_wolf_pos_update_dim_alpha(size_t dimension, double a, const double *wolf, const double *alpha_pos);


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
void gwo_update_wolf_position(size_t dim,
                              double a,
                              double *wolf,
                              const double *alpha_pos,
                              const double *beta_pos,
                              const double *delta_pos);

/**
   Update the positions of all wolves.
   TODO: test
 */
void gwo_update_all_positions(size_t wolf_count,
                              size_t dim,
                              double a,
                              double *population,
                              size_t alpha, size_t beta, size_t delta);


/**
   Clamp `val` between `min` and `max`.
 */
double gwo_clamp(double val, double min, double max);

/**
   Clamp solutions into feasible space.
 */
void gwo_clamp_all_positions(size_t wolf_count, size_t dim, double *population,
                             const double *min_positions, const double *max_positions);

size_t gwo_get_fittest_idx(size_t colony_size, const double * fitness);

#ifdef __cplusplus
}
#endif // __cplusplus
