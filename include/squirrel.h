#pragma once

#ifdef __cplusplus
extern "C" {
#endif


double* squirrel (double(*fit)(const double* const, size_t),
                  size_t population,
                  size_t dim,
                  size_t max_iter,
                  const double* const min_positions,
                  const double* const max_positions);
/**
* Randomly initialize squirrel population
**/
double* squirrel_rand_init(size_t population,
                  size_t dim,
                  const double* const min_positions,
                  const double* const max_positions);

/**
*   returns 1 with probability passed as arguement [0,1]
*   returns 0 with probability ( 1 - probability)
**/
int squirrel_bernoulli_distribution(double probability);

/**
*   Evaluate fitness at current positions
*   and update in array fitness
**/
void squirrel_eval_fitness(double (*func)(const double* const, size_t),
                          size_t population,
                          size_t dim,
                          const double* const positions,
                          double* fitness);
/**
*   find the lowest value and store
*   at the beginning of the array
**/
void squirrel_sort1(double* fitness,
                    double* positions,
                    size_t population,
                    size_t dim);
/**
*   find the 4 lowest values and
*   store them in the first four places in the array
**/
void squirrel_sort4(double* fitness,
                    double* positions,
                    size_t population,
                    size_t dim);
/**
* Calculate gliding distance
**/
double squirrel_gliding_dist();
/**
*   move squirrels from acorn and
*   select normal trees to hickory tree
**/
void move_to_hickory(double* positions,
                      size_t population,
                      size_t dim,
                      const double* const min_positions,
                      const double* const max_positions);
/**
*   move squirrels on normal tree towards acorn tree
**/
void move_normal_to_acorn(double* positions,
                          size_t population,
                          size_t dim,
                          const double* const min_positions,
                          const double* const  max_positions);
/**
*   check for boundary overflow
*   NOT IMPLEMENTED YET, SEEMS TO WORK WITHOUT
**/
void squirrel_enforce_bounds(double* positions,
                            size_t population,
                            size_t dim,
                            const double* const min_positions,
                            const double* const  max_positions);
/**
*   Evalulate seasonal_const to check for change of seasonal
**/
double squirrel_eval_seasonal_cons(double* positions, size_t dim);

/**
*   Calculate min seasonal constant
*   threshold for change of season
**/
double squirrel_eval_smin(size_t iter);

/**
*   Calculate factorial of a positive integer
**/
double squirrel_factorial(size_t n);

/**
*   Calculate factorial of non-integer values
**/
double squirrel_eval_gamma(double x);

/**
*  Levy flight based random step for
*  random restart/relocation after season change
**/
double squirrel_levy_flight();

/**
*   if season has changed,
*   relocate squirrels that hevent
*   travelled towards the hickory tree
**/
void random_restart(double* positions,
                    size_t population,
                    size_t dim,
                    const double* const min_positions,
                    const double* const max_positions);


#ifdef __cplusplus
}
#endif
