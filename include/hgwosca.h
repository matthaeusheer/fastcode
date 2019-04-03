#pragma once

#include "stddef.h"

double* const hgwosca(double(*obj)(const double* const, size_t),
                      size_t wolf_count,
                      size_t dim,
                      size_t max_iterations,
                      const double* const min_positions,
                      const double* const max_positions);


/**
   Initialise population of `wolf_count` wolves, each with `dim` dimensions, where
   each dimension is bound by `min_positions` and `max_positions`.
 */
double* const init_population(size_t wolf_count, size_t dim,
                                     const double* const min_positions,
                                     const double* const max_positions);

/**
   Update the fitness of all wolves.
 */
void update_fitness(size_t wolf_count, size_t dim,
                           double(*obj)(const double* const, size_t),
                           double* const population, double* const fitness);

/**
   Find the new wolf leaders.
 */
void update_leaders(size_t wolf_count, double* const fitness,
                           size_t* const alpha,
                           size_t* const beta,
                           size_t* const delta);


/**
   Initialise the fitness for all wolves.
 */
double* const init_fitness(size_t wolf_count, size_t dim,
                                  double(*obj)(const double* const, size_t),
                                  double* const population);


/**
   Get recommended position of `dimension` for `wolf` with respect to `leader`, for a given
   value of `a`.
   TODO: test
 */
double get_wolf_pos_update_dim_leader(const size_t dimension, const double a,
                                             const double* const wolf,
                                             const double* const leader_pos);


/**
   Get recommended position of `dimension` for `wolf` with respect to the alpha, for a given
   value of `a`. See equ (12) on the Hybrid paper.
   TODO: test
 */
double get_wolf_pos_update_dim_alpha(const size_t dimension, const double a,
                                            const double* const wolf,
                                            const double* const alpha_pos);


/**
   Update the position of `wolf` with respect to leaders `alpha`, `beta`, and `delta`, for a
   given value of `a`.
   TODO: test
 */
void update_wolf_position(const size_t dim, const double a,
                                 double* const wolf,
                                 const double* const alpha_pos,
                                 const double* const beta_pos,
                                 const double* const delta_pos);

/**
   Update the positions of all wolves.
   TODO: test
 */
void update_all_positions(const size_t wolf_count, const size_t dim,
                                 const double a, double* const population,
                                 size_t alpha, size_t beta, size_t delta);


/**
   Clamp `val` between `min` and `max`.
 */
double clamp(const double val, const double min, const double max);

/**
   Clamp solutions into feasible space.
 */
void clamp_all_positions(const size_t wolf_count, const size_t dim,
                                double* const population,
                                const double* const min_positions,
                                const double* const max_positions);