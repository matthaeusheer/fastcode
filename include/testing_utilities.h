#pragma once

#include <stdlib.h>

#include "utils.h"

/**
   Test an algorithm by running it on a some objective function.

   Args:
     obj_func:  the objective function to pass to the tested algorithm
     pop_size:  the population size to use for the test
     dim:       the dimension to use for the test
     min_bound: the lower bound on the solution space
     max_bound: the upper bound on the solution space
     max_iter:  the maximum number of iterations to run the algorithm
     algo:      the algorithm to test
     target:    the target objective function to reach
     tolerance: the accepted inaccuracy/distance from the target
     debug:     whether to print additional debug information
                this includes the best solution and the objective function
                value at that best solution
 */
void test_algo(obj_func_t obj_func, size_t pop_size, size_t dim,
               float min_bound, float max_bound,
               size_t max_iter, algo_func_t algo,
               float target, float tolerance,
               bool debug);
