/*****************************
BASIC PSO UNIT TESTS
*****************************/

#include <float.h>
#include <math.h>

#include "objectives.h"
#include "pso.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(pso_unit,pso_rand_init){
  srand((unsigned) time(NULL));
  size_t swarm_size = 10;
  size_t dim = 4;
  double min = 0.0;
  double max = 1.0;
  double* const pos = pso_rand_init(swarm_size, dim, min, max);
  for (size_t s=0;s<swarm_size * dim;s++){
    cr_assert_leq(pos[s], max, "dimension should be bound above");
    cr_assert_geq(pos[s], min, "dimension should be bound below");
  }
  free(pos);
}

Test(pso_unit,pso_eval_fitness){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double x[] = {
    0.0,  0.0,
    2.5,  4.0,
   10.0,  3.0,
  -10.0, -3.0
  };
  double * fitness = (double*)malloc(sizeof(double)*swarm_size);
  pso_eval_fitness(sum_of_squares, swarm_size, dim, x, fitness);
  cr_assert_float_eq(fitness[0], 0.0, DBL_EPSILON, "first particle fitness should be 0.0");
  cr_assert_float_eq(fitness[1], 22.25, DBL_EPSILON, "second particle fitness should be 22.5");
  cr_assert_float_eq(fitness[2], 109.0, DBL_EPSILON, "third particle's fitness should be 109");
  cr_assert_float_eq(fitness[3], 109.0, DBL_EPSILON, "fourth particle's fitness should be 109");

  free(fitness);
}

Test(pso_unit,pso_gen_init_velocity){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double min_vel[] = {0.0 , -1 , 1.0 , -10.0};
  double max_vel[] = {1.0 , 1.0, 3.0 , 25.0};
  double min = 0.0;
  double max = 1.0;

  double* const x = pso_rand_init(swarm_size, dim, min, max);
  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = pso_gen_init_velocity(x, swarm_size, dim, min, max);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = s*dim + d;
      cr_assert_leq(vel[idx], (max_vel[d] - x[idx])*0.5, "%ld dimension should be bound above",d);
      cr_assert_geq(vel[idx], (min_vel[d] - x[idx])*0.5, "%ld dimension should be bound below",d);
    }
  }
  free(x);
  free(vel);
}

Test(pso_unit,pso_best_fitness){
  size_t swarm_size,dim;
  swarm_size = 4;
  dim = 2;
  double local_best_position[] = {
    100.0,  0.0,
      2.5,  4.0,
     10.0,  3.0,
    -10.0, -3.0
  };

  double * fitness = (double*)malloc(sizeof(double)*swarm_size);
  pso_eval_fitness(sum_of_squares, swarm_size, dim, local_best_position, fitness);

  double* global_best_position;
  size_t best_index;
  best_index = pso_best_fitness(fitness, dim, swarm_size);
  cr_assert(best_index == 1, "the optima is at local_best_position[1]");

  global_best_position = local_best_position+(dim*best_index);
  cr_assert(global_best_position[0] == 2.5, "first dim of global best is 2.5");
  cr_assert(global_best_position[1] == 4.0, "second dim of global best is 4.0");

  free(fitness);
}

Test(pso_unit,pso_update_velocity){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double min_vel = -1.;
  double max_vel =  1.;
  double min = 0.0;
  double max = 1.0;

  double* const x = pso_rand_init(swarm_size, dim, min, max);
  double* const y = pso_rand_init(swarm_size, dim, min, max);
  double* const best = pso_rand_init(1, dim, min, max);

  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = pso_gen_init_velocity(x, swarm_size, dim, min, max);
  pso_update_velocity(vel, x, y, best, swarm_size, dim, min_vel, max_vel);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++) {
      size_t idx = s*dim + d;
        cr_assert_leq(vel[idx], max_vel ,"%ld dimension'd velocity should be bound above",d);
        cr_assert_geq(vel[idx], min_vel ,"%ld dimension'd velocity should be bound below",d);
    }
  }
  free(vel);
  free(x);
  free(y);
  free(best);
}

Test(pso_basic,pso_update_position){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double min = 0.0;
  double max = 1.0;
  double min_vel = -1.;
  double max_vel =  1.;

  double* const x = pso_rand_init(swarm_size, dim, min, max);
  double* const y = pso_rand_init(swarm_size, dim, min, max);
  double* const best = pso_rand_init(1, dim, min, max);

  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = pso_gen_init_velocity(x, swarm_size, dim, min, max);
  pso_update_velocity(vel, x, y, best, swarm_size, dim, min_vel, max_vel);
  pso_update_position( x, vel, swarm_size, dim, min, max);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++) {
      size_t idx = s*dim + d;
      cr_assert_leq(x[idx], max ,"%ld dimension'd position should be bound above",d);
      cr_assert_geq(x[idx], min ,"%ld dimension'd position should be bound below",d);
    }
  }
  free(vel);
  free(x);
  free(y);
  free(best);
}
