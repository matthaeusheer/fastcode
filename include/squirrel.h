#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "utils.h"

double* squirrel (obj_func_t obj_func,
                  size_t population,
                  size_t dim,
                  size_t max_iter,
                  const double min_position,
                  const double max_position);
/**
* Randomly initialize squirrel population
**/
double* sqr_rand_init(size_t population,
                  size_t dim,
                  const double min_position,
                  const double max_position);

/**
*   returns 1 with probability passed as arguement [0,1]
*   returns 0 with probability ( 1 - probability)
**/
int sqr_bernoulli_distribution(double probability);

/**
*   Evaluate fitness at current positions
*   and update in array fitness
**/
void sqr_eval_fitness(obj_func_t obj_func,
                      size_t population,
                      size_t dim,
                      const double* const positions,
                      double* fitness);
/**
*   find the lowest value and store
*   at the beginning of the array
**/
void sqr_lowest_val_to_front(double* fitness,
                    double* positions,
                    size_t population,
                    size_t dim);
/**
*   find the 4 lowest values and
*   store them in the first four places in the array
**/
void sqr_lowest4_vals_to_front(double* fitness,
                    double* positions,
                    size_t population,
                    size_t dim);
/**
* Calculate gliding distance
**/
double sqr_gliding_dist();
/**
*   move squirrels from acorn and
*   select normal trees to hickory tree
**/
void sqr_move_to_hickory(double* positions,
                      size_t population,
                      size_t dim,
                      const double min_position,
                      const double max_position);
/**
*   move squirrels on normal tree towards acorn tree
**/
void sqr_move_normal_to_acorn(double* positions,
                          size_t population,
                          size_t dim,
                          const double min_position,
                          const double max_position);
/**
*   Evalulate seasonal_const to check for change of seasonal
**/
double sqr_eval_seasonal_cons(double* positions, size_t dim);

/**
*   Calculate min seasonal constant
*   threshold for change of season
**/
double sqr_eval_smin(size_t iter);

/**
*   Calculate factorial of a positive integer
**/
double sqr_factorial(size_t n);

/**
*   Calculate factorial of non-integer values
**/
double sqr_eval_gamma(double x);

/**
*  Levy flight based random step for
*  random restart/relocation after season change
**/
double sqr_levy_flight();

/**
*   if season has changed,
*   relocate squirrels that hevent
*   travelled towards the hickory tree
**/
void random_restart(double* positions,
                    size_t population,
                    size_t dim,
                    const double min_position,
                    const double max_position);


#ifdef __cplusplus
}
#endif
