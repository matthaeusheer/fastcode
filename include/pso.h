#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
   Seed a parallel floating point RNG.
 */
void seed_simd_rng();

/**
   Generate a vector of random floats between `min` and `max`.
 */
inline __m256 simd_rand_min_max(float min, float max);

/**
    Initialise an array to random numbers between `min` and `max`.

    Arguments:
      array   the array to randomly initialise
      length  the length of the array to initialise
      min     minimum allowed value of each entry
      max     maximum allowed value of each entry
*/
void pso_rand_init(float *const array, size_t length,
                    const float min, const float max);

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
                      const float *positions, float *fitness);

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
void pso_gen_init_velocity(float *const velocity, const float *positions,
                          size_t swarm_size, size_t dim,
                          const float min_position,
                          const float max_position);

/**
   Returns the index of the particle with lowest fitness.

   Arguments:
     fitness     array containing the fitness values for all particles
     swarm_size  number of particles in fitness array

   Returns:
     A `size_t` representing the index.
 */
size_t pso_best_fitness(float *fitness, size_t swarm_size);


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
void pso_update_velocity(float *velocity, float *positions,
                         float *local_best_position,
                         float *best, size_t swarm_size, size_t dim,
                         const float min_vel,
                         const float max_vel);

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
void pso_update_bests(float *local_best_fitness, float *current_fitness,
                      float *local_best_positions, float *current_positions,
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
void pso_update_position(float *positions, float *velocity,
                         size_t swarm_size, size_t dim,
                         const float min_position,
                         const float max_position);

/**
   PSO algorithm.
 */
float *pso_basic(obj_func_t obj_func,
                  size_t swarm_size,
                  size_t dim, size_t max_iter,
                  const float min_position,
                  const float max_position);

#ifdef __cplusplus
}
#endif
