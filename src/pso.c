#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>

#include "pso.h"
#include "utils.h"


#define EPS 0.001
#define COG 2
#define SOCIAL 2
#define INERTIA 0.5
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

  //returns swarm_size * dim size array
double* rand_init(size_t swarm_size,
                  size_t dim,
                  const double* const min_positions,
                  const double* const max_positions) {
  double* init = (double*)malloc(swarm_size*dim*sizeof(double));
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      init[idx] = random_min_max(min_positions[d],max_positions[d]);
    }
  }
  return init;
}

  //returns swarm_size size array
void eval_fitness(double (*func)(const double* const, size_t),
                  size_t swarm_size, size_t dim,
                  const double* const x, double* fitness) {
  for (size_t pop=0; pop<swarm_size; pop++){
    fitness[pop] = func(x+(pop*dim),dim);
  }
  return ;
}

  //returns swarm_size x dim size array
  // velocity at every posize_t of every particle
double* gen_init_velocity(const double* const x,
                          size_t swarm_size, size_t dim,
                          const double* const min_positions,
                          const double* const max_positions) {
  double* u = (double *)malloc(swarm_size*dim*sizeof(double));
  u = rand_init(swarm_size,dim,min_positions,max_positions);
  double* v = (double *)malloc(swarm_size*dim*sizeof(double));
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      v[idx] = 0.25*(u[idx] - x[idx]);
    }
  }
  return v;
}

  // sorts the population in decreasing order of fitness /
  // i.e pop[0] posize_ts to best location yet.
size_t best_fitness(double* fitness,size_t dim, size_t swarm_size) {
  // find minimum of fitness
  // set index of mimima as minima_idx;
  // global_best = x+(dim*minima_idx);
  size_t minima_idx = 0;
  double best_yet = fitness[0]; // global best at timestep t+1
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
void generate_vel_limit(const double* min_positions,
                        const double* max_positions,
                        double* min_vel,
                        double* max_vel,
                        size_t dim) {
  for (size_t d=0;d<dim;d++){
    min_vel[d] = min_positions[d]/5;
    max_vel[d] = max_positions[d]/5;
  }
}

  // updates velocities, works on array returned by gen_velocity
double* update_velocity(double * v, double* x, double * y,
                        double* best, size_t swarm_size, size_t dim,
                        const double* const min_vel,
                        const double* const max_vel) {
  double* vnew = (double*)malloc(dim*swarm_size*sizeof(double));
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      vnew[idx] = INERTIA*v[idx] + COG*random_0_to_1()*(y[idx] - x[idx]) +
      SOCIAL*random_0_to_1()*(best[d] * x[idx]);
      vnew[idx] = min(max(min_vel[d],vnew[idx]),max_vel[d]);
    }
  }
  return vnew;
}

void update_position(double* x, double* v,
                    size_t swarm_size, size_t dim,
                    const double* const min_positions,
                    const double* const max_positions) {
  for (size_t particle=0; particle < swarm_size; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      x[idx] += v[idx];
      if (x[idx] >= max_positions[d] || x[idx] <= min_positions[d]){
        v[idx] = -v[idx];
      }
      x[idx] = min(max(min_positions[d],x[idx]),max_positions[d]);
      //TODO: PROJECT ON BOUNDS - RATHER DO A BOUNCE THAN A SIMPLE PROJECT
    }
  }
  return;
}

  /**
  Particle Swarm Algorithm to find the global minima of objective function using a nature mimicking swarm
  **/
double * pso_basic (double(*fit)(const double* const, size_t),
                    size_t swarm_size,
                    size_t dim,
                    size_t max_iter,
                    const double* const min_positions,
                    const double* const max_positions) {
  srand((unsigned) time(NULL));
  // REPLACE WITH FIXED ZERO SEED

  // double known_optima = 0.;   // TODO : DEPENDENT ON OBJECTIVE FUNCTION
  double* min_vel = (double*)malloc(dim*sizeof(double));
  double* max_vel = (double*)malloc(dim*sizeof(double));
  generate_vel_limit(min_positions,max_positions,min_vel,max_vel,dim);

  size_t sizeof_position = dim*swarm_size*sizeof(double);
  // randomly initialise postions of swarm particles
  double* current_positions = (double*)malloc(sizeof_position);
  current_positions = rand_init(swarm_size, dim,min_positions,max_positions);
  double* local_best_position = (double*)malloc(sizeof_position);
  memcpy(local_best_position,current_positions, sizeof_position);

  // calculate the fitness at every position of the swarm
  size_t sizeof_fitness = swarm_size*sizeof(double);
  double* current_fitness = (double*)malloc(sizeof_fitness);
  eval_fitness(fit,swarm_size,dim,current_positions,current_fitness);
  double* local_best_fitness = (double*)malloc(sizeof_fitness);
  memcpy(local_best_fitness,current_fitness, sizeof_fitness );

  double* p_velocity = (double*)malloc(sizeof_position);
  p_velocity = gen_init_velocity(current_positions,swarm_size,dim,
  min_positions,max_positions);

  double* global_best_position = (double*)malloc(dim*sizeof(double));
  size_t global_best_idx = best_fitness(local_best_fitness,dim,swarm_size);

  global_best_position = (local_best_position + (dim*global_best_idx));
  double global_best_fitness = local_best_fitness[global_best_idx];

  size_t iter = 0;
  // double error = 1e3;
  while ( iter < max_iter) { // error > EPS &&

    p_velocity = update_velocity(p_velocity,current_positions,
    local_best_position,global_best_position,swarm_size,
    dim,min_vel,max_vel);

    update_position(current_positions,p_velocity,swarm_size,
        dim,min_positions,max_positions);

    eval_fitness(fit,swarm_size,dim,current_positions,current_fitness);

    // check local best fitness
    for (size_t p=0;p<swarm_size;p++){
      if (current_fitness[p] < local_best_fitness[p]){
        local_best_fitness[p] = current_fitness[p];
        for (int d=0;d<dim;d++){
          size_t idx = (p*dim) + d;
          local_best_position[idx] = current_positions[idx];
        }
      }
    }

    global_best_idx = best_fitness(local_best_fitness,dim,swarm_size);
    global_best_position = local_best_position+(dim*global_best_idx);
    global_best_fitness  = local_best_fitness[global_best_idx];
    iter++;

  }
  return global_best_position;
}