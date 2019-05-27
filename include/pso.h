#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DIM (128)
#define SIMD_DIM (16)

/**
   TODO: update doc
 */
typedef struct {
  __m256 velocity[SIMD_DIM];
  __m256 position[SIMD_DIM];
  float fitness;
  float best_fit;
  __m256 best[SIMD_DIM];
} Particle;

/**
   Seed a parallel floating point RNG.
 */
void seed_simd_rng();

/**
   Generate a vector of random floats between `min` and `max`.
 */
inline __m256 simd_rand_min_max();


/**
   Initialise velocity bounds for later use.
 */
void initialise_velocity_bounds(float min_vel, float max_vel);


/**
    Initialise position bounds for later use.
 */
void initialise_position_bounds(float min_position, float max_position);

/**
    Generate a vector of random floats between 0 and 1.
 */
inline __m256 simd_rand_0_to_1();

/**
    Initialise an array to random numbers between `min` and `max`.

    Arguments:
      array   the array to randomly initialise
      length  the length of the array to initialise
*/
size_t pso_init_swarm(simd_obj_func_t obj_func, Particle *swarm, size_t swarm_size);


size_t update_everything(Particle *swarm, __m256 *global_best_position,
                         float *global_best_fitness, __m256 *new_global_best,
                         simd_obj_func_t obj_func,
                         size_t swarm_size);

/**
   PSO algorithm.
 */
float *pso_basic(simd_obj_func_t obj_func, size_t swarm_size, size_t dim,
                 size_t max_iter, const float min_position, const float max_position);

#ifdef __cplusplus
}
#endif
