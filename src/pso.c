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

  //returns swarm_size * dim size array
void pso_rand_init(float* const population,
                   size_t swarm_size,
                   size_t dim,
                   const float min_position,
                   const float max_position) {
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      population[idx] = random_min_max(min_position,max_position);
    }
  }
}

  //returns swarm_size size array
void pso_eval_fitness(obj_func_t obj_func,
                      size_t swarm_size, size_t dim,
                      const float* const positions, float* fitness) {
  for (size_t pop=0; pop<swarm_size; pop++){
    fitness[pop] = obj_func(positions+(pop*dim),dim);
  }
  return ;
}

  //returns swarm_size x dim size array
  // velocity at every posize_t of every particle
void pso_gen_init_velocity(float* const velocity,
                            const float* const positions,
                            size_t swarm_size, size_t dim,
                            const float min_position,
                            const float max_position) {
  float u[swarm_size * dim];
  pso_rand_init(u,swarm_size,dim,min_position,max_position);
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      velocity[idx] = 0.25*(u[idx] - positions[idx]);
    }
  }
}

  // sorts the population in decreasing order of fitness /
  // i.e pop[0] posize_ts to best location yet.
size_t pso_best_fitness(float* fitness,size_t dim, size_t swarm_size) {
  // find minimum of fitness
  // set index of mimima as minima_idx;
  // global_best = x+(dim*minima_idx);
  size_t minima_idx = 0;
  float best_yet = fitness[0]; // global best at timestep t+1
  for (size_t p=1;p<swarm_size;p++){
    if (fitness[p]<best_yet) {
      best_yet = fitness[p];
      minima_idx = p;
    }
  }
  // global_best_position = local_best_position+(dim*minima_idx);
  return minima_idx;
}

  // limit max and min velocity values to ensure no boundary overflow
void pso_generate_vel_limit(const float min_position,
                            const float max_position,
                            float* min_vel,
                            float* max_vel) {
    *min_vel = min_position/VEL_LIMIT_SCALE;
    *max_vel = max_position/VEL_LIMIT_SCALE;
}

  // updates velocities, works on array returned by gen_velocity
  //REMOVE VNEW WORK WITH VELOCITY
void pso_update_velocity(float* velocity, float* positions,
                         float* local_best_position,
                         float* best, size_t swarm_size, size_t dim,
                         const float min_vel,
                         const float max_vel) {
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      velocity[idx] = INERTIA*velocity[idx] + COG*random_0_to_1()*(local_best_position[idx] - positions[idx]) +
      SOCIAL*random_0_to_1()*(best[d] - positions[idx]);
      velocity[idx] = min(max(min_vel,velocity[idx]),max_vel);
    }
  }
  return ;
}

void pso_update_position(float* positions, float* velocity,
                         size_t swarm_size, size_t dim,
                         const float min_position,
                         const float max_position) {
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      positions[idx] += velocity[idx];
      if (positions[idx] >= max_position || positions[idx] <= min_position){
        velocity[idx] = -velocity[idx];
      }
      positions[idx] = min(max(min_position,positions[idx]),max_position);
    }
  }
  return;
}

  /**
  Particle Swarm Algorithm to find the global minima of objective function using a nature mimicking swarm
  **/
float * pso_basic(obj_func_t obj_func,
                   size_t swarm_size,
                   size_t dim,
                   size_t max_iter,
                   const float min_position,
                   const float max_position) {
  srand(100);

  float min_vel = 0;
  float max_vel = 0;
  pso_generate_vel_limit(min_position,max_position,&min_vel,&max_vel);

  size_t sizeof_position = dim*swarm_size*sizeof(float);
  // randomly initialise postions of swarm particles
  float* current_positions = (float*)malloc(sizeof_position);
  pso_rand_init(current_positions,swarm_size, dim,min_position,max_position);
  float* local_best_position = (float*)malloc(sizeof_position);
  memcpy(local_best_position,current_positions, sizeof_position);

  // calculate the fitness at every position of the swarm
  size_t sizeof_fitness = swarm_size*sizeof(float);
  float* current_fitness = (float*)malloc(sizeof_fitness);
  pso_eval_fitness(obj_func,swarm_size,dim,current_positions,current_fitness);
  float* local_best_fitness = (float*)malloc(sizeof_fitness);
  memcpy(local_best_fitness, current_fitness, sizeof_fitness);

  #ifdef DEBUG
      print_population(swarm_size, dim, current_positions); // printing the initial status of the population
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, current_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  float* p_velocity = (float*)malloc(sizeof_position);
  pso_gen_init_velocity(p_velocity,current_positions,swarm_size,dim,min_position,max_position);

  size_t global_best_idx = pso_best_fitness(local_best_fitness,dim,swarm_size);

  float* global_best_position = (local_best_position + (dim*global_best_idx));
  float global_best_fitness = local_best_fitness[global_best_idx];

  size_t iter = 0;
  while ( iter < max_iter) {

    pso_update_velocity(p_velocity,current_positions,
    local_best_position,global_best_position,swarm_size,
    dim,min_vel,max_vel);

    pso_update_position(current_positions,p_velocity,swarm_size,
        dim,min_position,max_position);

    pso_eval_fitness(obj_func,swarm_size,dim,current_positions,current_fitness);

    // check local best fitness
    for (size_t p=0;p<swarm_size;p++){
      if (current_fitness[p] < local_best_fitness[p]){
        local_best_fitness[p] = current_fitness[p];
        for (size_t d=0;d<dim;d++){
          size_t idx = (p*dim) + d;
          local_best_position[idx] = current_positions[idx];
        }
      }
    }

    global_best_idx = pso_best_fitness(local_best_fitness,dim,swarm_size);
    global_best_position = local_best_position+(dim*global_best_idx);
    global_best_fitness  = local_best_fitness[global_best_idx];
    iter++;

  #ifdef DEBUG
      print_population(swarm_size, dim, current_positions); // printing the initial status of the population
      printf("# AVG FITNESS: %f\n", average_value(swarm_size, local_best_fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(swarm_size, local_best_fitness));
  #endif

  }

  float* const best_solution = (float *const) malloc(dim);
  memcpy(best_solution, global_best_position , dim*sizeof(float));

   free(current_positions);
   free(local_best_position);
   //free(current_fitness);
   //free(local_best_fitness);
   free(p_velocity);

  return best_solution;
}
