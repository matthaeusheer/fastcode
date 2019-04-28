#pragma once

#include <string>
#include <utility>
#include <map>

#include "utils.h"
#include "objectives.h"


// Algorithm function type
typedef double * (*algo_func_t)(obj_func_t, size_t, size_t, size_t, const double *, const double *);

// String to objective function pointer type
typedef std::map<std::string, obj_func_t> obj_map_t;

// String to algorithm function pointer type
typedef std::map<std::string, algo_func_t> algo_map_t;

/**
 * Main function to run an algorithm on a function and time it.
 */
std::vector<unsigned long long> time_algorithm(Config cfg);

/**
 * Builds up the mapping of identifier (used in config) to objective function pointer.
 */
obj_map_t create_obj_map();

/**
 * Builds up the mapping of identifier (used in config) to algorithm function pointer.
 */
algo_map_t create_algo_map();
