#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

double * hgwosca(double(*obj)(const double *const, size_t),
                 size_t wolf_count,
                 size_t dim,
                 size_t max_iterations,
                 const double *min_positions,
                 const double *max_positions);

/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
double *init_population(size_t wolf_count, size_t dim, const double * min_positions, const double * max_positions);

/**
   Update the fitness of all wolves.
 */
void update_fitness(size_t wolf_count,
                    size_t dim,
                    double(*obj)(const double *const, size_t),
                    double *population,
                    double *fitness);

/**
   Find the new wolf leaders.
 */
void update_leaders(size_t wolf_count, double *fitness, size_t *alpha, size_t *beta, size_t *delta);

/**
   Initialise the fitness for all wolves.
 */
double *init_fitness(size_t wolf_count, size_t dim, double(*obj)(const double *const, size_t), double *population);

/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
double get_wolf_pos_update_dim_leader(size_t dimension, double a, const double *wolf, const double *leader_pos);

/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
double get_wolf_pos_update_dim_alpha(size_t dimension, double a, const double *wolf, const double *alpha_pos);


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
void update_wolf_position(size_t dim,
                          double a,
                          double *wolf,
                          const double *alpha_pos,
                          const double *beta_pos,
                          const double *delta_pos);

/**
   Update the positions of all wolves.
   TODO: test
 */
void update_all_positions(size_t wolf_count,
                          size_t dim,
                          double a,
                          double *population,
                          size_t alpha, size_t beta, size_t delta);


/**
   Clamp `val` between `min` and `max`.
 */
double clamp(double val, double min, double max);

/**
   Clamp solutions into feasible space.
 */
void clamp_all_positions(size_t wolf_count, size_t dim, double *population,
                         const double *min_positions, const double *max_positions);

/**
   Prints the population to standard output.
 */
void gwo_print_pop(size_t colony_size, size_t dim, const double * population);

#ifdef __cplusplus
}
#endif // __cplusplus