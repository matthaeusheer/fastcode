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
  cr_skip_test();
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
      cr_expect_leq(tmp[idx], max, "dimension should be bound above %f", tmp[idx]);
      cr_expect_geq(tmp[idx], min, "dimension should be bound below %f", tmp[idx]);
    }
  }
}

Test(pso_unit, pso_eval_fitness) {
  cr_skip_test();
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
  /* pso_eval_fitness(sum_of_squares, swarm_size, dim / 8, x, fitness); */
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
