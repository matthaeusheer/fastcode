#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "utils.h"

float* squirrel (obj_func_t obj_func,
                  size_t population,
                  size_t dim,
                  size_t max_iter,
                  const float min_position,
                  const float max_position);
/**
* Randomly initialize squirrel population
**/
void sqr_rand_init(float* const positions,
                  size_t population,
                  size_t dim,
                  const float min_position,
                  const float max_position);

/**
*   returns 1 with probability passed as arguement [0,1]
*   returns 0 with probability ( 1 - probability)
**/
int sqr_bernoulli_distribution(float probability);

/**
*   Evaluate fitness at current positions
*   and update in array fitness
**/
void sqr_eval_fitness(obj_func_t obj_func,
                      size_t population,
                      size_t dim,
                      const float* const positions,
                      float* fitness);
/**
*   find the lowest value and store
*   at the beginning of the array
**/
void sqr_lowest_val_to_front(float* fitness,
                    float* positions,
                    size_t population,
                    size_t dim);
/**
*   find the 4 lowest values and
*   store them in the first four places in the array
**/
void sqr_lowest4_vals_to_front(float* fitness,
                    float* positions,
                    size_t population,
                    size_t dim);
/**
* Calculate gliding distance
**/
float sqr_gliding_dist();
/**
*   move squirrels from acorn and
*   select normal trees to hickory tree
**/
void sqr_move_to_hickory(float* positions,
                      size_t population,
                      size_t dim,
                      const float min_position,
                      const float max_position);
/**
*   move squirrels on normal tree towards acorn tree
**/
void sqr_move_normal_to_acorn(float* positions,
                          size_t population,
                          size_t dim,
                          const float min_position,
                          const float max_position);
/**
*   Evalulate seasonal_const to check for change of seasonal
**/
float sqr_eval_seasonal_cons(float* positions, size_t dim);

/**
*   Calculate min seasonal constant
*   threshold for change of season
**/
float sqr_eval_smin(size_t iter);

/**
*   Calculate factorial of a positive integer
**/
float sqr_factorial(size_t n);

/**
*   Calculate factorial of non-integer values
**/
float sqr_eval_gamma(float x);

/**
*  Levy flight based random step for
*  random restart/relocation after season change
**/
float sqr_levy_flight();

/**
*   if season has changed,
*   relocate squirrels that hevent
*   travelled towards the hickory tree
**/
void random_restart(float* positions,
                    size_t population,
                    size_t dim,
                    const float min_position,
                    const float max_position);


#ifdef __cplusplus
}
#endif
