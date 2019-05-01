#pragma once

#include <stdlib.h>

#include "utils.h"

void test_algo(obj_func_t obj_func, size_t pop_size, size_t dim,
               double min_bound, double max_bound,
               size_t max_iter, algo_func_t algo,
               double target, double tolerance,
               bool debug);
