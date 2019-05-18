#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>
#include <limits.h>

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

__m256i seed_a;
__m256i seed_b;
__m256 max_rng;

__m256 factor_0_to_1;
__m256 middle_0_to_1;

__m256 inertia;
__m256 cog;
__m256 social;

/**
   Seed a parallel floating point RNG.
 */
void seed_simd_rng(size_t seed) {
  srand(seed);
  seed_a = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());
  seed_b = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());
  max_rng = _mm256_cvtepi32_ps(_mm256_set1_epi32(INT_MIN));

  inertia = _mm256_set1_ps(INERTIA);
  cog = _mm256_set1_ps(COG);
  social = _mm256_set1_ps(SOCIAL);

  factor_0_to_1 = _mm256_set1_ps(0.5);
  middle_0_to_1 = _mm256_set1_ps(0.5);
}

/**
   Generate a vector of random floats between `min` and `max`.
 */
inline __m256 simd_rand_min_max(float min, float max) {
  float factor = (max - min) / 2;
  float middle = min + factor;

  __m256i s1 = seed_a;
	const __m256i s0 = seed_b;
	seed_a = seed_b;
	s1 = _mm256_xor_si256(seed_b, _mm256_slli_epi64(seed_b, 23));
	seed_b = _mm256_xor_si256(_mm256_xor_si256(_mm256_xor_si256(s1, s0),
                                             _mm256_srli_epi64(s1, 18)),
                            _mm256_srli_epi64(s0, 5));
  __m256i rands = _mm256_add_epi64(seed_b, s0);
  __m256 frands = _mm256_div_ps(_mm256_cvtepi32_ps(rands), max_rng);
  __m256 factors = _mm256_set1_ps(factor);
  __m256 middles = _mm256_set1_ps(middle);
  return _mm256_fmadd_ps(factors, frands, middles);
}

/**
   Generate a vector of random floats between 0 and 1.
*/
inline __m256 simd_rand_0_to_1() {
  __m256i s1 = seed_a;
	const __m256i s0 = seed_b;
	seed_a = seed_b;
	s1 = _mm256_xor_si256(seed_b, _mm256_slli_epi64(seed_b, 23));
	seed_b = _mm256_xor_si256(_mm256_xor_si256(_mm256_xor_si256(s1, s0),
                                             _mm256_srli_epi64(s1, 18)),
                            _mm256_srli_epi64(s0, 5));
  __m256i rands = _mm256_add_epi64(seed_b, s0);
  __m256 frands = _mm256_div_ps(_mm256_cvtepi32_ps(rands), max_rng);
  return _mm256_fmadd_ps(factor_0_to_1, frands, middle_0_to_1);
}

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
  size_t idx = 0;
  if(length > 7) {
    for(; idx < length - 8; idx += 8) {
      _mm256_storeu_ps(&array[idx], simd_rand_min_max(min, max));
    }
  }
  for(; idx < length; idx++) {
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
  float* u = (float*)malloc(swarm_size * dim * sizeof(float));
  if (!u) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_rand_init(u, swarm_size * dim, min_position, max_position);
  __m256 quarter = _mm256_set1_ps(0.25);

  size_t idx = 0;
  for(; idx < swarm_size * dim - 8; idx += 8) {
    __m256 pos = _mm256_loadu_ps(&positions[idx]);
    __m256 v_u = _mm256_loadu_ps(&u[idx]);
    __m256 diff = _mm256_sub_ps(v_u, pos);
    _mm256_storeu_ps(&velocity[idx], _mm256_mul_ps(quarter, diff));
  }

  for(; idx < swarm_size * dim; idx++) {
    velocity[idx] = 0.25 * (u[idx] - positions[idx]);
  }
  free(u);
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
  __m256 v_max_vel = _mm256_set1_ps(max_vel);
  __m256 v_min_vel = _mm256_set1_ps(min_vel);

  for(size_t particle = 0; particle < swarm_size; particle++) {
    size_t dimension = 0;
    if(dim > 7) {
      for(; dimension < dim - 8; dimension += 8) {
        size_t idx = (particle * dim) + dimension;
        __m256 vel = _mm256_loadu_ps(&velocity[idx]);
        __m256 rand1 = simd_rand_0_to_1();
        __m256 rand2 = simd_rand_0_to_1();
        __m256 local_best_pos = _mm256_loadu_ps(&local_best_positions[idx]);
        __m256 pos = _mm256_loadu_ps(&positions[idx]);
        __m256 best_pos = _mm256_loadu_ps(&best[dimension]);

        __m256 term1 = _mm256_mul_ps(rand1, _mm256_sub_ps(local_best_pos, pos));
        __m256 term2 = _mm256_mul_ps(rand2, _mm256_sub_ps(best_pos, pos));
        __m256 res = _mm256_mul_ps(inertia, vel);
        res = _mm256_fmadd_ps(cog, term1, res);
        res = _mm256_fmadd_ps(social, term2, res);

        res = _mm256_min_ps(_mm256_max_ps(v_min_vel, res), v_max_vel);

        _mm256_storeu_ps(&velocity[idx], res);
      }
    }

    for(; dimension < dim; dimension++) {
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
      size_t dimension = 0;
      if(dim > 7) {
        for(; dimension < dim - 8; dimension += 8) {
          size_t idx = (particle * dim) + dimension;
          _mm256_storeu_ps(&local_best_positions[idx], _mm256_loadu_ps(&current_positions[idx]));
        }
      }
      for(; dimension < dim; dimension++) {
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
  __m256 v_min = _mm256_set1_ps(min_position);
  __m256 v_max = _mm256_set1_ps(max_position);

  size_t idx = 0;
  for(; idx < swarm_size * dim - 8; idx += 8) {
    __m256 pos = _mm256_loadu_ps(&positions[idx]);
    __m256 vel = _mm256_loadu_ps(&velocity[idx]);
    pos = _mm256_add_ps(pos, vel);
    pos = _mm256_min_ps(_mm256_max_ps(v_min, pos), v_max);
    _mm256_storeu_ps(&positions[idx], pos);
  }

  for(; idx < swarm_size * dim; idx++) {
    positions[idx] += velocity[idx];
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
  seed_simd_rng(100);

  float min_vel = min_position/VEL_LIMIT_SCALE;
  float max_vel = max_position/VEL_LIMIT_SCALE;

  size_t sizeof_position = dim * swarm_size * sizeof(float);
  float *current_positions = (float*)malloc(sizeof_position);
  if (!current_positions) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_rand_init(current_positions, swarm_size * dim, min_position, max_position);
  float *local_best_positions = (float*)malloc(sizeof_position);
  if (!local_best_positions) { perror("malloc arr"); exit(EXIT_FAILURE); };
  memcpy(local_best_positions, current_positions, sizeof_position);

  size_t sizeof_fitness = swarm_size * sizeof(float);
  float *current_fitness = (float*)malloc(sizeof_fitness);
  if (!current_fitness) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_eval_fitness(obj_func, swarm_size, dim, current_positions, current_fitness);

  float *local_best_fitness = (float*)malloc(sizeof_fitness);
  if (!local_best_fitness) { perror("malloc arr"); exit(EXIT_FAILURE); };
  memcpy(local_best_fitness, current_fitness, sizeof_fitness);

  #ifdef DEBUG
      print_population(swarm_size, dim, current_positions);
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, current_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  float *p_velocity = (float*)malloc(sizeof_position);
  if (!p_velocity) { perror("malloc arr"); exit(EXIT_FAILURE); };

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

  float *const best_solution = (float *const)malloc(dim * sizeof(float));
  if (!best_solution) { perror("malloc arr"); exit(EXIT_FAILURE); };

  memcpy(best_solution, global_best_position , dim * sizeof(float));

  free(current_positions);
  free(local_best_positions);
  free(current_fitness);
  free(local_best_fitness);
  free(p_velocity);

  return best_solution;
}
