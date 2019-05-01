#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>

#include "squirrel.h"
#include "utils.h"

#define NUM_JUMP_HICK 0.2
#define T_MAX 20
#define PREDATOR_PROB 0.1
#define BETA 1.5
#define GLIDING_CONST 1.9
#define CD 0.60
#define CL_MIN 0.675
#define CL_MAX 1.5
#define SF 18
#define DROP 8

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef SQRT_PI
#define SQRT_PI 1.77245385091
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


double* sqr_rand_init(size_t population,
                  size_t dim,
                  const double* const min_positions,
                  const double* const max_positions) {
  double* init = (double*)malloc(population*dim*sizeof(double));
  for (size_t particle=0; particle < population; particle++){
    for (size_t d=0; d<dim; d++){
      size_t idx = (particle*dim) + d;
      init[idx] = random_min_max(min_positions[d],max_positions[d]);
    }
  }
  return init;
}


int sqr_bernoulli_distribution(double probability){
  if (probability < 0 || probability > 1) return -1;

  if (random_0_to_1() < probability ) return 1;
  return 0;
}

void sqr_eval_fitness(obj_func_t obj_func,
                      size_t population, size_t dim,
                      const double* const positions,
                      double* fitness) {
  for (size_t pop=0; pop<population; pop++){
    fitness[pop] = obj_func(positions+(pop*dim),dim);
  }
  return ;
}


void sqr_lowest_val_to_front(double* fitness,double* positions, size_t population, size_t dim){
  double best_yet = fitness[0];
  size_t best_idx = 0;

  for (size_t pop_idx=1; pop_idx<population; pop_idx++){
    if (fitness[pop_idx] < best_yet){
      best_yet = fitness[pop_idx];
      best_idx = pop_idx;
    }
  }

  double temp = fitness[0];
  fitness[0] = fitness[best_idx];
  fitness[best_idx] = temp;

  // careful
  double* temp_pos = (double*)malloc( dim*sizeof(double));

  memcpy(temp_pos, positions, dim*sizeof(double));
  memcpy(positions, positions+(dim*best_idx), dim*sizeof(double));
  memcpy(positions +(dim*best_idx), temp_pos, dim*sizeof(double));


  free(temp_pos);
  return;
}

  /**
  *   sort the fitness and position arrays
  *   so that the best four squirrels
  *   are the first four elements
  **/
void sqr_lowest4_vals_to_front(double* fitness,double* positions, size_t population, size_t dim){
  for (size_t i = 0; i < 4; i++) {
    sqr_lowest_val_to_front(fitness+i,positions+i*dim,population-i,dim);
  }
}

double sqr_gliding_dist(){
  double lift = random_min_max(CL_MIN,CL_MAX);
  double drag = CD;
  //double phi = atan(drag/lift);
  return DROP/(SF*drag/lift);
}

void sqr_move_to_hickory(double* positions,
                    size_t population,
                    size_t dim,
                    const double* const min_positions,
                    const double* const max_positions){
  // pop > 0 && pop < 4
  double p = PREDATOR_PROB;
  if (!sqr_bernoulli_distribution(p)){
    for (size_t pop_idx = 1; pop_idx < 4+NUM_JUMP_HICK*population ; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        positions[idx] = positions[idx] +
                        sqr_gliding_dist()*GLIDING_CONST*(positions[d]-positions[idx]);
      }
    }
  } else {
    for (size_t pop_idx = 1; pop_idx < 4+NUM_JUMP_HICK*population ; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        positions[idx] = random_min_max(min_positions[d],max_positions[d]);
      }
    }
  }
  return;
}

void sqr_move_normal_to_acorn(double* positions,
                          size_t population,
                          size_t dim,
                          const double* const min_positions,
                          const double* const  max_positions){
// pop >= 4+ NUM_JUMP_HICK*population
  double p = PREDATOR_PROB;
  if (!sqr_bernoulli_distribution(p)){
    for (size_t pop_idx = 4+NUM_JUMP_HICK*population; pop_idx < population; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        size_t acorn_idx = ( 1 + ( idx % 3))*dim + d;
        positions[idx] = positions[idx] +
                    sqr_gliding_dist()*GLIDING_CONST*(positions[acorn_idx]-positions[idx]);
      }
    }
  } else {
    for (size_t pop_idx = 4+NUM_JUMP_HICK*population; pop_idx < population; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        positions[idx] = random_min_max(min_positions[d],max_positions[d]);
      }
    }
  }
  return;
}

double sqr_eval_seasonal_cons(double* positions, size_t dim){
  double s_c_2 = 0;
  for (size_t pop_idx = 1; pop_idx < 4; pop_idx ++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = pop_idx*dim + d;
      s_c_2 += pow( (positions[idx] - positions[d]),2);
    }
  }
  return sqrt(s_c_2);
}

double sqr_eval_smin(size_t iter){
return 1e-6/( pow( 365, ( 2.5*iter/T_MAX)));
}

double sqr_factorial(size_t n){
  if (n>=1) return n*sqr_factorial(n-1);
  else return 1;
}

double sqr_eval_gamma(double x){
  if (x >= 1) {
  size_t n = floor(x);
  return SQRT_PI*
        ( sqr_factorial(2*n-1) )/( sqr_factorial(n-1)*pow(2,(2*n-1)) );
  } else {
    size_t n = fabs(floor(x));
    return pow(-4,n)*sqr_factorial(n)*SQRT_PI/sqr_factorial(2*n);
  }
}

double sqr_levy_flight(){
  double sigma = pow( ( ( sqr_eval_gamma( 1+BETA)*sin( 0.5*M_PI*BETA))/
                 ( sqr_eval_gamma( 0.5*( 1+BETA))*BETA*pow( 2,( 0.5*( BETA-1))))),( 1/BETA));

  return 0.01*random_0_to_1()*sigma/pow( random_0_to_1(),( 1/BETA) );
}

void random_restart(double* positions,size_t population, size_t dim, const double* const min_positions, const double* const max_positions){
  for (size_t pop_idx = 4+NUM_JUMP_HICK*population; pop_idx < population; pop_idx ++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = pop_idx*dim + d;
      positions[idx] =  min_positions[d] +
                      sqr_levy_flight()*(max_positions[d]-min_positions[d]);
    }
  }
  return;
}


double* squirrel (obj_func_t obj_func,
                  size_t population,
                  size_t dim,
                  size_t max_iter,
                  const double* const min_positions,
                  const double* const max_positions) {
  srand((unsigned) time(NULL));

  double p_dp = PREDATOR_PROB;
  size_t num_jump_hick = ceil(NUM_JUMP_HICK*population);

  double* positions = sqr_rand_init(population,dim,min_positions,max_positions);
  size_t sizeof_position = dim*population*sizeof(double);

  size_t sizeof_fitness = population*sizeof(double);
  double* fitness = (double*)malloc(sizeof_fitness);
  sqr_eval_fitness(obj_func,population,dim,positions,fitness);

  // positions[0] is hickory, positions[1:3] are acorn, rest are normal.
  sqr_lowest4_vals_to_front(fitness,positions,population,dim);

  #ifdef DEBUG
    print_population(population, dim, positions); // printing the initial status of the population
    printf("# AVG FITNESS: %f\n", average_value(population, fitness));
  #endif

  double s_c = 0;
  size_t iter = 0;
  double s_min = sqr_eval_smin(iter);                                // seasonal constant
  while (iter < max_iter) {
    iter++;

    sqr_move_to_hickory(positions,population,dim,min_positions,max_positions);
    sqr_move_normal_to_acorn(positions,population,dim,min_positions,max_positions);

    s_c = sqr_eval_seasonal_cons(positions, dim);
    if (s_c < s_min){
      random_restart(positions,population,dim,min_positions,max_positions);
    }
    s_min = sqr_eval_smin(iter);

    sqr_eval_fitness(obj_func,population,dim,positions,fitness);
    sqr_lowest4_vals_to_front(fitness,positions,population,dim);

    #ifdef DEBUG
    #endif

    #ifdef DEBUG
      print_population(population, dim, positions); // printing the initial status of the population
      printf("# AVG FITNESS: %f\n", average_value(population, fitness));
      printf("\n # iter: %lu best val: %f\n", iter, fitness[0] );
    #endif
  }




  double* const best_solution = (double *const) malloc(dim * sizeof(double));
  memcpy(best_solution, positions , dim * sizeof(double));

  free(fitness);
  free(positions);

  return best_solution;
}
