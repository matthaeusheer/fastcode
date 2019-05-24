#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
   PSO algorithm.
 */
double *pso_basic(obj_func_t obj_func,
                 size_t swarm_size,
                 size_t dim, size_t max_iter,
                 const double min_position,
                 const double max_position);

/**
    Initialise an array to random numbers between `min` and `max`.

    Arguments:
      array   the array to randomly initialise
      length  the length of the array to initialise
      min     minimum allowed value of each entry
      max     maximum allowed value of each entry
*/
void pso_rand_init(double *const array, size_t length,
                    const double min, const double max);

/**
   Evaluate fitness of `positions` according to `obj_func` and store the result
   in `fitness`.

    Arguments:
     obj_func   objective function with which to compute the fitness
     swarm_size number of particles for which to compute the fitness
     dim        dimension of the position of each particle
     positions  position array of the particles
     fitness    array where to store the result
 */
void pso_eval_fitness(obj_func_t obj_func,
                      size_t swarm_size, size_t dim,
                      const double *positions, double *fitness);

/**
   Randomly generate initial velocity for all particles.

   Arguments:
     velocity      array where to store the result
     positions     position array of the particles
     swarm_size    number of particles for which to compute the initial velocity
     dim           dimension of the position of each particle
     max_position  upper bound on each entry of the position for a particle
     min_position  lower bound on each entry of the position for a particle
 */
void pso_gen_init_velocity(double *const velocity, const double *positions,
                          size_t swarm_size, size_t dim,
                          const double min_position,
                          const double max_position);

/**
   Returns the index of the particle with lowest fitness.

   Arguments:
     fitness     array containing the fitness values for all particles
     swarm_size  number of particles in fitness array

   Returns:
     A `size_t` representing the index.
 */
size_t pso_best_fitness(double *fitness, size_t swarm_size);


/**
   Update the velocity of each particle in the swarm.

   Arguments:
     velocity              the array to update
     positions             positions of the particles in the swarm
     local_best_positions  previous best positions of each particle
     best                  best solution so far
     swarm_size            number of particles in the swarm
     dim                   dimension of a single particle
     min_vel               minimum allowed value for each velocity entry
     max_vel               maximum allowed value for each velocity entry
 */
void pso_update_velocity(double *velocity, double *positions,
                         double *local_best_position,
                         double *best, size_t swarm_size, size_t dim,
                         const double min_vel,
                         const double max_vel);

/**
   Update the local best fitness and position for all particles.

   Arguments:
     local_best_fitness    local best fitness of the particles
     current_fitness       current fitness of the particles
     local_best_positions  local best positions of the particles
     current_positions     current positions of the particles
     swarm_size            number of particles in the swarm]
     dim                   dimension of a single particle
 */
void pso_update_bests(double *local_best_fitness, double *current_fitness,
                      double *local_best_positions, double *current_positions,
                      size_t swarm_size, size_t dim);

/**
   Update the position for all particles in the swarm.

   Arguments:
     positions     positions of the particles in the swarm
     velocity      velocity of the particles in the swarm
     swarm_size    number of particles in the swarm
     dim           dimension of a single particle
     min_position  lower bound on each entry of the position for a particle
     max_position  upper bound on each entry of the position for a particle
 */
void pso_update_position(double *positions, double *velocity,
                         size_t swarm_size, size_t dim,
                         const double min_position,
                         const double max_position);
#ifdef __cplusplus
}
#endif
