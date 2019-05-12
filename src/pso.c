#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>

#include "pso.h"
#include "utils.h"


#define EPS 0.001
#define COG 0.5
#define SOCIAL .9
#define INERTIA 0.5
#define VEL_LIMIT_SCALE 5
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/**
   Initialise an array to random numbers between `min` and `max`.

   Arguments:
     array   the array to randomly initialise
     length  the length of the array to initialise
     min     minimum allowed value of each entry
     max     maximum allowed value of each entry
 */
void pso_rand_init(float *const array, size_t length,
                   const float min, const float max) {
  for(size_t idx = 0; idx < length; idx++) {
    array[idx] = random_min_max(min, max);
  }
}

/**
   Evaluate fitness of `positions` according to `obj_func` and store the result
   in `fitness`.

   Arguments:
     obj_func    objective function with which to compute the fitness
     swarm_size  number of particles for which to compute the fitness
     dim         dimension of the position of each particle
     positions   position array of the particles
     fitness     array where to store the result
 */
void pso_eval_fitness(obj_func_t obj_func,
                      size_t swarm_size, size_t dim,
                      const float *const positions, float *fitness) {
  for(size_t particle = 0; particle < swarm_size; particle++) {
    fitness[particle] = obj_func(&positions[particle * dim], dim);
  }
}

/**
   Randomly generate initial velocity for all particles.

   Arguments:
     velocity      array where to store the result
     positions     position array of the particles
     swarm_size    number of particles for which to compute the initial velocity
     dim           dimension of the position of each particle
     min_position  lower bound on each entry of the position for a particle
     max_position  upper bound on each entry of the position for a particle
 */
void pso_gen_init_velocity(float *const velocity, const float *const positions,
                            size_t swarm_size, size_t dim,
                            const float min_position,
                            const float max_position) {
  float u[swarm_size * dim];
  pso_rand_init(u, swarm_size * dim, min_position, max_position);
  for(size_t idx = 0; idx < swarm_size * dim; idx++) {
    velocity[idx] = 0.25 * (u[idx] - positions[idx]);
  }
}

/**
   Returns the index of the particle with lowest fitness.

   Arguments:
     fitness     array containing the fitness values for all particles
     swarm_size  number of particles in fitness array

   Returns:
     A `size_t` representing the index.
 */
size_t pso_best_fitness(float *fitness, size_t swarm_size) {
  size_t min_idx = 0;
  float min = fitness[0];
  for(size_t particle = 1; particle < swarm_size; particle++) {
    if(fitness[particle] < min) {
      min = fitness[particle];
      min_idx = particle;
    }
  }
  return min_idx;
}

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
                         float *local_best_positions,
                         float *best, size_t swarm_size, size_t dim,
                         const float min_vel,
                         const float max_vel) {
  for(size_t particle = 0; particle < swarm_size; particle++) {
    for(size_t dimension = 0; dimension < dim; dimension++) {
      size_t idx = (particle * dim) + dimension;
      velocity[idx] = INERTIA * velocity[idx] +\
          COG * random_0_to_1() * (local_best_positions[idx] - positions[idx]) +\
          SOCIAL * random_0_to_1() * (best[dimension] - positions[idx]);
      velocity[idx] = min(max(min_vel, velocity[idx]), max_vel);
    }
  }
}

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
                      size_t swarm_size, size_t dim) {
  for(size_t particle = 0; particle < swarm_size; particle++) {
    if(current_fitness[particle] < local_best_fitness[particle]) {
      local_best_fitness[particle] = current_fitness[particle];
      for(size_t dimension = 0; dimension < dim; dimension++) {
        size_t idx = (particle * dim) + dimension;
        local_best_positions[idx] = current_positions[idx];
      }
    }
  }
}

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
                         const float max_position) {
  for(size_t idx = 0; idx < swarm_size * dim; idx++) {
    positions[idx] += velocity[idx];
    if(positions[idx] >= max_position || positions[idx] <= min_position) {
      velocity[idx] = -velocity[idx];
    }
    positions[idx] = min(max(min_position, positions[idx]), max_position);
  }
}

/**
   PSO algorithm.
 */
float *pso_basic(obj_func_t obj_func,
                 size_t swarm_size,
                 size_t dim,
                 size_t max_iter,
                 const float min_position,
                 const float max_position) {
  srand(100);

  float min_vel = min_position/VEL_LIMIT_SCALE;
  float max_vel = max_position/VEL_LIMIT_SCALE;

  size_t sizeof_position = dim * swarm_size * sizeof(float);
  float *current_positions = (float*)malloc(sizeof_position);
  pso_rand_init(current_positions, swarm_size * dim, min_position, max_position);
  float *local_best_positions = (float*)malloc(sizeof_position);
  memcpy(local_best_positions, current_positions, sizeof_position);

  size_t sizeof_fitness = swarm_size * sizeof(float);
  float *current_fitness = (float*)malloc(sizeof_fitness);
  pso_eval_fitness(obj_func, swarm_size, dim, current_positions, current_fitness);
  float *local_best_fitness = (float*)malloc(sizeof_fitness);
  memcpy(local_best_fitness, current_fitness, sizeof_fitness);

  #ifdef DEBUG
      print_population(swarm_size, dim, current_positions);
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, current_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  float *p_velocity = (float*)malloc(sizeof_position);
  pso_gen_init_velocity(p_velocity, current_positions, swarm_size, dim,
                        min_position, max_position);

  size_t global_best_idx = pso_best_fitness(local_best_fitness, swarm_size);
  float *global_best_position = &local_best_positions[dim * global_best_idx];
  float global_best_fitness = local_best_fitness[global_best_idx];

  for(size_t iter = 0; iter < max_iter; iter++) {
    pso_update_velocity(p_velocity, current_positions, local_best_positions,
                        global_best_position, swarm_size, dim, min_vel, max_vel);

    pso_update_position(current_positions, p_velocity, swarm_size, dim,
                        min_position, max_position);

    pso_eval_fitness(obj_func, swarm_size, dim, current_positions,
                     current_fitness);

    pso_update_bests(local_best_fitness, current_fitness, local_best_positions,
                     current_positions, swarm_size, dim);

    global_best_idx = pso_best_fitness(local_best_fitness, swarm_size);
    global_best_position = local_best_positions + (dim * global_best_idx);
    global_best_fitness = local_best_fitness[global_best_idx];

  #ifdef DEBUG
      print_population(swarm_size, dim, current_positions);
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, local_best_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  }

  float *const best_solution = (float *const)malloc(dim);
  memcpy(best_solution, global_best_position , dim * sizeof(float));

  free(current_positions);
  free(local_best_positions);
  free(current_fitness);
  free(local_best_fitness);
  free(p_velocity);

  return best_solution;
}
