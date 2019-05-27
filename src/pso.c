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
#include "objectives.h"


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
   TODO: update doc
   Initialise an array to random numbers between `min` and `max`.

   Arguments:
     array   the array to randomly initialise
     length  the length of the array to initialise in terms of __m256
 */
size_t pso_init_swarm(simd_obj_func_t obj_func, Particle *const swarm, size_t swarm_size) {
  size_t min_idx = 0;
  float min_fit = FLT_MAX;
  for (size_t idx = 0; idx < swarm_size; idx++) {
    for (size_t dim = 0; dim < SIMD_DIM; dim++) {
      __m256 rand_pos = simd_rand_min_max();
      __m256 rand_vel = simd_rand_min_max();
      __m256 diff = _mm256_sub_ps(rand_vel, rand_pos);
      swarm[idx].position[dim] = rand_pos;
      swarm[idx].velocity[dim] = _mm256_mul_ps(quarter, diff);
      swarm[idx].best[dim] = rand_pos;
    }
    float fitness = obj_func(swarm[idx].position, SIMD_DIM);
    swarm[idx].fitness = fitness;
    swarm[idx].best_fit = fitness;
    if (fitness < min_fit) {
      min_fit = fitness;
      min_idx = idx;
    }
  }
  return min_idx;
}


/**
   TODO: update doc
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
size_t update_everything(Particle *const swarm, __m256 *global_best_position,
                         float *global_best_fitness, __m256 *new_global_best,
                         simd_obj_func_t obj_func,
                         size_t swarm_size) {
  size_t size_position = SIMD_DIM * sizeof(__m256);
  size_t min_idx = 0;
  float min_fit = FLT_MAX;
  for (size_t idx = 0; idx < swarm_size; idx++) {
    for (size_t dim = 0; dim < SIMD_DIM; dim++) {
      __m256 rand1 = simd_rand_0_to_1();
      __m256 rand2 = simd_rand_0_to_1();
      __m256 term1 = _mm256_mul_ps(rand1, _mm256_sub_ps(swarm[idx].best[dim], swarm[idx].position[dim]));
      __m256 term2 = _mm256_mul_ps(rand2, _mm256_sub_ps(global_best_position[dim], swarm[idx].position[dim]));
      __m256 res = _mm256_mul_ps(inertia, swarm[idx].velocity[dim]);
      res = _mm256_fmadd_ps(cog, term1, res);
      res = _mm256_fmadd_ps(social, term2, res);
      res = _mm256_min_ps(_mm256_max_ps(v_min_vel, res), v_max_vel);
      swarm[idx].velocity[dim] = res;
      swarm[idx].position[dim] = _mm256_add_ps(swarm[idx].position[dim], swarm[idx].position[dim]);
      swarm[idx].position[dim] = _mm256_min_ps(_mm256_max_ps(v_min_pos, swarm[idx].position[dim]), v_max_vel);
    }
    swarm[idx].fitness = obj_func(swarm[idx].position, SIMD_DIM);
    if (swarm[idx].fitness < min_fit) {
      min_fit = swarm[idx].fitness;
      min_idx = idx;
    }
    if (swarm[idx].fitness < swarm[idx].best_fit) {
      swarm[idx].best_fit = swarm[idx].best_fit;
      memcpy(swarm[idx].best, swarm[idx].position, size_position);
    }
    if (swarm[idx].fitness < *global_best_fitness) {
      *global_best_fitness = swarm[idx].fitness;
      memcpy(new_global_best, swarm[idx].position, size_position);
    }
  }
  return min_idx;
}


/**
   PSO algorithm.
 */
float *pso_basic(simd_obj_func_t obj_func, size_t swarm_size, size_t dim,
                 size_t max_iter, const float min_position, const float max_position) {
  assert(dim == 128);
  assert(swarm_size % 8 == 0);

  init_obj_globals();

  seed_simd_rng(100);

  float min_vel = min_position/VEL_LIMIT_SCALE;
  float max_vel = max_position/VEL_LIMIT_SCALE;

  initialise_velocity_bounds(min_vel, max_vel);
  initialise_position_bounds(min_position, max_position);

  size_t sizeof_all = swarm_size * sizeof(Particle);
  size_t sizeof_position = SIMD_DIM * sizeof(__m256);
  Particle* swarm = (Particle*)malloc(sizeof_all);
  if (!swarm) { perror("malloc arr"); exit(EXIT_FAILURE); };
  size_t global_best_idx = pso_init_swarm(obj_func, swarm, swarm_size);
  __m256 *global_best_position = (__m256*)malloc(sizeof_position);
  __m256 *new_global_best_pos = (__m256*)malloc(sizeof_position);

  memcpy(global_best_position, swarm[global_best_idx].position, sizeof_position);

  float global_best_fitness = swarm[global_best_idx].fitness;

  for(size_t iter = 0; iter < max_iter; iter++) {
    global_best_idx = update_everything(swarm, global_best_position, &global_best_fitness,
                                        new_global_best_pos, obj_func, swarm_size);

    __m256 *tmp = global_best_position;
    global_best_position = new_global_best_pos;
    new_global_best_pos = tmp;
  }

  float *const best_solution = (float *const)malloc(dim * sizeof(float));
  if (!best_solution) { perror("malloc arr"); exit(EXIT_FAILURE); };

  // TODO copy this properly
  for(size_t idx = 0; idx < SIMD_DIM; idx++) {
    _mm256_storeu_ps(&best_solution[idx * 8], global_best_position[idx]);
  }
  /* memcpy(best_solution, global_best_position , dim * sizeof(float)); */

  free(swarm);
  free(global_best_position);
  free(new_global_best_pos);

  return best_solution;
}
