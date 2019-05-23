#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

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
__m256 mul_factor;

__m256 factor_min_to_max;

__m256 inertia;
__m256 cog;
__m256 social;

__m256 quarter;

__m256 v_min_vel;
__m256 v_max_vel;

__m256 v_min_pos;
__m256 v_max_pos;

/**
   Seed a parallel floating point RNG.
 */
void seed_simd_rng(size_t seed) {
  srand(seed);
  seed_a = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());
  seed_b = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());

  mul_factor = _mm256_set1_ps(1.0f / 2147483648.0f);

  inertia = _mm256_set1_ps(INERTIA);
  cog = _mm256_set1_ps(COG);
  social = _mm256_set1_ps(SOCIAL);

  quarter = _mm256_set1_ps(0.25);
}

void initialise_velocity_bounds(float min_vel, float max_vel) {
  v_min_vel = _mm256_set1_ps(min_vel);
  v_max_vel = _mm256_set1_ps(max_vel);
}

void initialise_position_bounds(float min_position, float max_position) {
  v_min_pos = _mm256_set1_ps(min_position);
  v_max_pos = _mm256_set1_ps(max_position);
  factor_min_to_max = _mm256_sub_ps(v_max_pos, v_min_pos);
}


/**
   Generate a vector of random floats between `min` and `max`.
 */
inline __m256 simd_rand_min_max() {
  const __m256 rands = simd_rand_0_to_1();
  return _mm256_fmadd_ps(rands, factor_min_to_max, v_min_pos);
}

/**
   Generate a vector of random floats between 0 and 1.
*/
inline __m256 simd_rand_0_to_1() {
  seed_a = seed_b;

  const __m256i s0 = seed_b;
  const __m256i s1 = _mm256_xor_si256(seed_b, _mm256_slli_epi64(seed_b, 23));

  const __m256i lhs = _mm256_xor_si256(_mm256_xor_si256(s1, s0), _mm256_srli_epi64(s1, 18));
  const __m256i rhs = _mm256_srli_epi64(s0, 5);

  seed_b = _mm256_xor_si256(lhs, rhs);
  const __m256 rands = _mm256_cvtepi32_ps(_mm256_abs_epi32(_mm256_add_epi64(seed_b, s0)));
  return _mm256_mul_ps(rands, mul_factor);
}

/**
   Initialise an array to random numbers between `min` and `max`.

   Arguments:
     array   the array to randomly initialise
     length  the length of the array to initialise in terms of __m256
 */
void pso_rand_init(__m256 *const array, size_t length) {
  for(size_t idx = 0; idx < length; idx++) {
    array[idx] = simd_rand_min_max();
  }
}

/**
   Evaluate fitness of `positions` according to `obj_func` and store the result
   in `fitness`.

   Arguments:
     obj_func    objective function with which to compute the fitness
     swarm_size  number of particles for which to compute the fitness
     simd_dim    dimension of the position of each particle in terms of __m256
     positions   position array of the particles
     fitness     array where to store the result
 */
void pso_eval_fitness(simd_obj_func_t obj_func,
                      size_t swarm_size, size_t simd_dim,
                      const __m256 *const positions, float *fitness) {
  for(size_t particle = 0; particle < swarm_size; particle++) {
    fitness[particle] = obj_func(&positions[particle], simd_dim);
  }
}

/**
   Randomly generate initial velocity for all particles.

   Arguments:
     velocity      array where to store the result
     positions     position array of the particles
     swarm_size    number of particles for which to compute the initial velocity
     simd_dim      dimension of the position of each particle in terms of __m256
 */
void pso_gen_init_velocity(__m256 *const velocity, const __m256 *const positions,
                           size_t swarm_size, size_t simd_dim) {
  __m256* u = (__m256*)malloc(swarm_size * simd_dim * sizeof(__m256));
  if (!u) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_rand_init(u, swarm_size * simd_dim);

  for(size_t idx = 0; idx < swarm_size * simd_dim; idx++) {
    __m256 diff = _mm256_sub_ps(u[idx], positions[idx]);
    velocity[idx] = _mm256_mul_ps(quarter, diff);
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
   Update the velocity, positions, local best positions, global best positions,
   fitness, and local best fitness of each particle in the swarm.

   Arguments:
   velocity              the array to update
   positions             positions of the particles in the swarm
   local_best_positions  previous best positions of each particle
   globa_best_position   best solution so far
   current_fitness       fitness of particles
   local_best_fitess     local best fitness of each particle
   obj_func              objective function used to compute fitnesss
   swarm_size            number of particles in the swarm
   simd_dim              dimension of a single particle in terms of __m256
 */
void update_everything(__m256 *velocity, __m256 *positions,
                       __m256 *local_best_positions,
                       __m256 *global_best_position,
                       float *current_fitness, float* local_best_fitness,
                       simd_obj_func_t obj_func,
                       size_t swarm_size, size_t simd_dim) {
  for(size_t particle = 0; particle < swarm_size; particle++) {
    // update velocity for particle
    for(size_t dimension = 0; dimension < simd_dim; dimension++) {
      size_t idx = (particle * simd_dim) + dimension;
      __m256 rand1 = simd_rand_0_to_1();
      __m256 rand2 = simd_rand_0_to_1();
      __m256 term1 = _mm256_mul_ps(rand1, _mm256_sub_ps(local_best_positions[idx], positions[idx]));
      __m256 term2 = _mm256_mul_ps(rand2, _mm256_sub_ps(global_best_position[dimension], positions[idx]));
      __m256 res = _mm256_mul_ps(inertia, velocity[idx]);
      res = _mm256_fmadd_ps(cog, term1, res);
      res = _mm256_fmadd_ps(social, term2, res);

      res = _mm256_min_ps(_mm256_max_ps(v_min_vel, res), v_max_vel);

      velocity[idx] = res;
    }

    // update position for particle
    for(size_t dimension = 0; dimension < simd_dim; dimension++) {
      size_t idx = (particle * simd_dim) + dimension;
      positions[idx] = _mm256_add_ps(positions[idx], velocity[idx]);
      positions[idx] = _mm256_min_ps(_mm256_max_ps(v_min_pos, positions[idx]), v_max_pos);
    }

    // update fitness for particle
    current_fitness[particle] = obj_func(&positions[particle], simd_dim);

    // update local best fitness and position for particle
    if(current_fitness[particle] < local_best_fitness[particle]) {
      local_best_fitness[particle] = current_fitness[particle];
      for(size_t dimension = 0; dimension < simd_dim; dimension++) {
        size_t j = (particle * simd_dim) + dimension;
        local_best_positions[j] = positions[j];
      }
    }
  }
}


/**
   PSO algorithm.
 */
float *pso_basic(simd_obj_func_t obj_func,
                 size_t swarm_size,
                 size_t dim,
                 size_t max_iter,
                 const float min_position,
                 const float max_position) {
  assert(dim % 8 == 0);
  assert(swarm_size % 8 == 0);

  size_t simd_dim = dim / 8;

  seed_simd_rng(100);

  float min_vel = min_position/VEL_LIMIT_SCALE;
  float max_vel = max_position/VEL_LIMIT_SCALE;

  initialise_velocity_bounds(min_vel, max_vel);
  initialise_position_bounds(min_position, max_position);

  size_t sizeof_position = swarm_size * simd_dim * sizeof(__m256);
  __m256 *current_positions = (__m256*)malloc(sizeof_position);
  if (!current_positions) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_rand_init(current_positions, swarm_size * simd_dim);
  __m256 *local_best_positions = (__m256*)malloc(sizeof_position);
  if (!local_best_positions) { perror("malloc arr"); exit(EXIT_FAILURE); };
  memcpy(local_best_positions, current_positions, sizeof_position);
  __m256 *global_best_position = (__m256*)malloc(simd_dim * sizeof(__m256));

  size_t sizeof_fitness = swarm_size * sizeof(float);
  float *current_fitness = (float*)malloc(sizeof_fitness);
  if (!current_fitness) { perror("malloc arr"); exit(EXIT_FAILURE); };
  pso_eval_fitness(obj_func, swarm_size, simd_dim, current_positions, current_fitness);

  float *local_best_fitness = (float*)malloc(sizeof_fitness);
  if (!local_best_fitness) { perror("malloc arr"); exit(EXIT_FAILURE); };
  memcpy(local_best_fitness, current_fitness, sizeof_fitness);

  #ifdef DEBUG
      simd_print_population(swarm_size, dim, current_positions);
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, current_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  __m256 *p_velocity = (__m256*)malloc(sizeof_position);
  if (!p_velocity) { perror("malloc arr"); exit(EXIT_FAILURE); };

  pso_gen_init_velocity(p_velocity, current_positions, swarm_size, simd_dim);

  size_t global_best_idx = pso_best_fitness(local_best_fitness, swarm_size);
  memcpy(global_best_position, &local_best_positions[simd_dim * global_best_idx], simd_dim * sizeof(__m256));

  float global_best_fitness = local_best_fitness[global_best_idx];

  for(size_t iter = 0; iter < max_iter; iter++) {
    update_everything(p_velocity, current_positions, local_best_positions,
                      global_best_position, current_fitness, local_best_fitness,
                      obj_func, swarm_size, simd_dim);

    global_best_idx = pso_best_fitness(local_best_fitness, swarm_size);
    memcpy(global_best_position, &local_best_positions[simd_dim * global_best_idx], simd_dim * sizeof(__m256));

    global_best_fitness = local_best_fitness[global_best_idx];

  #ifdef DEBUG
      simd_print_population(swarm_size, dim, current_positions);
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, local_best_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  }

  float *const best_solution = (float *const)malloc(dim * sizeof(float));
  if (!best_solution) { perror("malloc arr"); exit(EXIT_FAILURE); };

  // TODO copy this properly
  for(size_t idx = 0; idx < simd_dim; idx++) {
    _mm256_storeu_ps(&best_solution[idx * 8], global_best_position[idx]);
  }
  /* memcpy(best_solution, global_best_position , dim * sizeof(float)); */

  free(current_positions);
  free(local_best_positions);
  free(current_fitness);
  free(local_best_fitness);
  free(p_velocity);
  free(global_best_position);

  return best_solution;
}
