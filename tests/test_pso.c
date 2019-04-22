/*****************************
BASIC PSO UNIT TESTS
*****************************/

#include <float.h>
#include <math.h>

#include "objectives.h"
#include "pso.h"
#include "utils.h"

#include <criterion/criterion.h>

Test(pso_unit,rand_init){
  srand((unsigned) time(NULL));
  size_t swarm_size = 10;
  size_t dim = 4;
  double mins[] = {0.0 , -1 , 1.0 , -10.0};
  double maxs[] = {1.0 , 1.0, 3.0 , 25.0};
  double* const pos = rand_init(swarm_size,dim,mins,maxs);
  for (size_t s=0;s<swarm_size;s++){
    cr_assert(pos[s * dim] <= maxs[0], "first dimension should be bound above");
    cr_assert(pos[s * dim] >= mins[0], "first dimension should be bound below");
    cr_assert(pos[s * dim + 1] <= maxs[1], "second dimension should be bound above");
    cr_assert(pos[s * dim + 1] >= mins[1], "second dimension should be bound below");
    cr_assert(pos[s * dim + 2] <= maxs[2], "third dimension should be bound above");
    cr_assert(pos[s * dim + 2] >= mins[2], "third dimension should be bound below");
    cr_assert(pos[s * dim + 3] <= maxs[3], "fourth dimension should be bound above");
    cr_assert(pos[s * dim + 3] >= mins[3], "fourth dimension should be bound below");
  }
}

Test(pso_unit,eval_fitness){
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
  eval_fitness(sum_of_squares, swarm_size, dim, x, fitness);
  cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first particle fitness should be 0");
  cr_assert(fabs(fitness[1] - 22.25) < DBL_EPSILON, "second particle fitness should be 17");
  cr_assert(fabs(fitness[2] - 109.0) < DBL_EPSILON, "third wolf's fitness should be 25");
  cr_assert(fabs(fitness[3] - 109.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");

}

Test(pso_unit,gen_init_velocity){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double mins[] = {0.0 , -1 , 1.0 , -10.0};
  double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

  double* const x = rand_init(swarm_size,dim,mins,maxs);
  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = s*dim + d;
      cr_assert(vel[idx] <= (maxs[d] - x[idx])* 0.5,"%ld dimension should be bound above",d);
      cr_assert(vel[idx] >= (mins[d] - x[idx])*0.5, "%ld dimension should be bound below",d);
    }
  }
}

Test(pso_unit,best_fitness){
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
  eval_fitness(sum_of_squares, swarm_size, dim, local_best_position, fitness);

  double* global_best_position = (double*)malloc(sizeof(double)*dim);
  size_t best_index;
  best_index = best_fitness(fitness,dim, swarm_size);
  cr_assert(best_index == 1, "the optima is at local_best_position[1]");
  global_best_position = local_best_position+(dim*best_index);

  cr_assert(global_best_position[0] == 2.5, "first dim of global best is 1");
  cr_assert(global_best_position[1] == 4.0, "second dim of global best is 0");

  free(fitness);
}

Test(pso_unit,update_velocity){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double mins[] = {0.0 , -1 , 1.0 , -10.0};
  double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

  double* const x = rand_init(swarm_size,dim,mins,maxs);
  double* const y = rand_init(swarm_size,dim,mins,maxs);
  double* const best = rand_init(1,dim,mins,maxs);

  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
  vel = update_velocity(vel, x, y, best, swarm_size, dim,
  mins, maxs);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++) {
      size_t idx = s*dim + d;
        cr_assert(vel[idx] <= maxs[d] ,"%ld dimension'd velocity should be bound above",d);
        cr_assert(vel[idx] >= mins[d] , "%ld dimension'd velocity should be bound below",d);
    }
  }
  free(vel);
}

Test(pso_basic,update_position){
  size_t swarm_size, dim;
  swarm_size = 4;
  dim = 2;
  double mins[] = {0.0 , -1 , 1.0 , -10.0};
  double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

  double* const x = rand_init(swarm_size,dim,mins,maxs);
  double* const y = rand_init(swarm_size,dim,mins,maxs);
  double* const best = rand_init(1,dim,mins,maxs);

  double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
  vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
  vel = update_velocity(vel, x, y, best, swarm_size, dim,
  mins, maxs);
  update_position( x, vel, swarm_size, dim, mins, maxs);
  for (size_t s=0;s<swarm_size;s++){
    for (size_t d = 0; d < dim; d++) {
      size_t idx = s*dim + d;
      cr_assert(x[idx] <= maxs[d] ,"%ld dimension'd position should be bound above",d);
      cr_assert(x[idx] >= mins[d] ,"%ld dimension'd position should be bound below",d);
    }
  }
}

/*
Test(pso_unit,pso_basic){

  size_t swarm_size = 50;
  size_t dim = 10;
  size_t max_iter = 2000;
  double* mins = filled_array(dim, -5);
  double* maxs = filled_array(dim, 5);
  double* solution = pso_basic(rastigrin, swarm_size, dim, max_iter, mins, maxs);

  cr_assert(fabs(rastigrin(solution, dim)) < 0.001, "objective should be minimised at 0");
  // free(solution);
  // free(mins);
  // free(maxs);
}
*/
