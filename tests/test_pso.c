/*****************************
BASIC PSO UNIT TESTS
*****************************/

#include <float.h>
#include <math.h>

#include "objectives.h"
#include "pso.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(pso_unit, pso_rand_init) {
  seed_simd_rng(100);
  float min = 0.0;
  float max = 1.0;
  initialise_velocity_bounds(min, max);
  initialise_position_bounds(min, max);
  size_t swarm_size = 16;
  size_t dim = 8;
  __m256 pos[swarm_size * dim / 8];
  pso_rand_init(pos, swarm_size * dim / 8);
  for(size_t s = 0; s < swarm_size * dim / 8; s++) {
    float tmp[8];
    _mm256_storeu_ps(tmp, pos[s]);
    for(size_t idx = 0; idx < 8; idx++) {
      cr_expect_leq(tmp[idx], max, "dimension should be bound above");
      cr_expect_geq(tmp[idx], min, "dimension should be bound below");
    }
  }
}

Test(pso_unit, pso_eval_fitness) {
  size_t swarm_size, dim;
  swarm_size = 8;
  dim = 8;
  __m256 x[] = {
      _mm256_set_ps(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(2.5, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(10.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(-10.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
      _mm256_set_ps(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
  };
  float fitness[8];
  pso_eval_fitness(sum_of_squares, swarm_size, dim / 8, x, fitness);
  cr_expect_float_eq(fitness[0], 0.0, FLT_EPSILON,
                     "first particle fitness should be 0.0");
  cr_expect_float_eq(fitness[1], 22.25, FLT_EPSILON,
                     "second particle fitness should be 22.5");
  cr_expect_float_eq(fitness[2], 109.0, FLT_EPSILON,
                     "third particle's fitness should be 109");
  cr_expect_float_eq(fitness[3], 109.0, FLT_EPSILON,
                     "fourth particle's fitness should be 109");
  cr_expect_float_eq(fitness[4], 0.0, FLT_EPSILON,
                     "fifth particle fitness should be 0.0");
  cr_expect_float_eq(fitness[5], 0.0, FLT_EPSILON,
                     "sixth particle fitness should be 0.0");
  cr_expect_float_eq(fitness[6], 0.0, FLT_EPSILON,
                     "seventh particle fitness should be 0.0");
  cr_expect_float_eq(fitness[7], 0.0, FLT_EPSILON,
                     "eighth particle fitness should be 0.0");
}

Test(pso_unit, pso_gen_init_velocity) {
  cr_skip_test();
  seed_simd_rng(100);
  float min = 0.0;
  float max = 1.0;
  initialise_velocity_bounds(min, max);
  initialise_position_bounds(min, max);

  size_t swarm_size = 8;
  size_t dim = 8;
  size_t simd_dim = dim / 8;
  float min_vel[] = {0.0, -1.0 , 1.0, -10.0, -10.0, -10.0, -10.0, -10.0};
  float max_vel[] = {1.0, 1.0, 3.0, 25.0, 10.0, 10.0, 10.0, 10.0};

  __m256 x[swarm_size * simd_dim];
  pso_rand_init(x, swarm_size * simd_dim);
  __m256 vel[swarm_size * simd_dim];
  pso_gen_init_velocity(vel, x, swarm_size, simd_dim);
  for(size_t s = 0; s < swarm_size; s++) {
    for(size_t d = 0; d < simd_dim; d++) {
      size_t idx = s * simd_dim + d;
      float tmp_vel[8];
      float tmp_x[8];
      _mm256_storeu_ps(tmp_vel, vel[idx]);
      _mm256_storeu_ps(tmp_x, x[idx]);
      for(size_t j = 0; j < 8; j++) {
        cr_expect_leq(tmp_vel[j], (max_vel[j] - tmp_x[j]) * 0.5,
                      "%ld dimension should be bound above", j);
        cr_expect_geq(tmp_vel[j], (min_vel[d] - tmp_x[j]) * 0.5,
                      "%ld dimension should be bound below", j);
      }
    }
  }
}

/* Test(pso_unit, pso_best_fitness) { */
/*   size_t swarm_size,dim; */
/*   swarm_size = 4; */
/*   dim = 2; */
/*   float local_best_position[] = { */
/*      100.0, 0.0, */
/*      2.5, 4.0, */
/*      10.0, 3.0, */
/*       -10.0, -3.0 */
/*   }; */

/*   float fitness[swarm_size]; */
/*   pso_eval_fitness(sum_of_squares, swarm_size, dim, local_best_position, fitness); */

/*   float* global_best_position; */
/*   size_t best_index; */
/*   best_index = pso_best_fitness(fitness, swarm_size); */
/*   cr_expect_eq(best_index, 1, "the optima is at local_best_position[1]"); */

/*   global_best_position = local_best_position+(dim*best_index); */
/*   cr_expect_float_eq(global_best_position[0], 2.5, FLT_EPSILON, */
/*                      "first dim of global best is 2.5"); */
/*   cr_expect_float_eq(global_best_position[1], 4.0, FLT_EPSILON, */
/*                      "second dim of global best is 4.0"); */
/* } */

/* Test(pso_unit, pso_update_velocity) { */
/*   seed_simd_rng(100); */
/*   size_t swarm_size, dim; */
/*   swarm_size = 4; */
/*   dim = 2; */
/*   float min_vel = -1.; */
/*   float max_vel =  1.; */
/*   float min = 0.0; */
/*   float max = 1.0; */

/*   float x[swarm_size * dim]; */
/*   pso_rand_init(x, swarm_size * dim, min, max); */
/*   float y[swarm_size * dim]; */
/*   pso_rand_init(y, swarm_size * dim, min, max); */
/*   float best[dim]; */
/*   pso_rand_init(best, dim, min, max); */

/*   float vel[swarm_size * dim]; */
/*   pso_gen_init_velocity(vel, x, swarm_size, dim, min, max); */
/*   pso_update_velocity(vel, x, y, best, swarm_size, dim, min_vel, max_vel); */
/*   for(size_t s = 0; s < swarm_size; s++) { */
/*     for(size_t d = 0; d < dim; d++) { */
/*       size_t idx = s * dim + d; */
/*       cr_expect_leq(vel[idx], max_vel, */
/*                     "%ld dimension'd velocity should be bound above", d); */
/*       cr_expect_geq(vel[idx], min_vel, */
/*                     "%ld dimension'd velocity should be bound below", d); */
/*     } */
/*   } */
/* } */

/* Test(pso_unit, pso_update_position) { */
/*   seed_simd_rng(100); */
/*   size_t swarm_size, dim; */
/*   swarm_size = 4; */
/*   dim = 2; */
/*   float min = 0.0; */
/*   float max = 1.0; */
/*   float min_vel = -1.; */
/*   float max_vel =  1.; */

/*   float x[swarm_size * dim]; */
/*   pso_rand_init(x, swarm_size * dim, min, max); */
/*   float y[swarm_size * dim]; */
/*   pso_rand_init(y, swarm_size * dim, min, max); */
/*   float best[dim]; */
/*   pso_rand_init(best, dim, min, max); */

/*   float vel[swarm_size * dim]; */
/*   pso_gen_init_velocity(vel, x, swarm_size, dim, min, max); */
/*   pso_update_velocity(vel, x, y, best, swarm_size, dim, min_vel, max_vel); */
/*   pso_update_position( x, vel, swarm_size, dim, min, max); */
/*   for(size_t s = 0; s < swarm_size; s++) { */
/*     for(size_t d = 0; d < dim; d++) { */
/*       size_t idx = s * dim + d; */
/*       cr_expect_leq(x[idx], max, */
/*                     "%ld dimension'd position should be bound above", d); */
/*       cr_expect_geq(x[idx], min, */
/*                     "%ld dimension'd position should be bound below", d); */
/*     } */
/*   } */
/* } */
