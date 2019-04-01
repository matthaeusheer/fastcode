#pragma once

#include <string>
#include <utility>

#include "utils.h"
#include "../objectives/objectives.h"


// Objective function type
typedef double (*obj_func_t) (const double* const, size_t);

// Algorithm function type
typedef double* const (*algo_func_t) (obj_func_t, size_t, size_t, size_t, const double* const, const double* const);

// String to objective function pointer type
typedef std::map<std::string, obj_func_t> obj_map_t;

// String to algorithm function pointer type
typedef std::map<std::string, algo_func_t > algo_map_t;

/**
 * Main function to run an algorithm on a function and time it.
 */
std::vector<unsigned long long> time_algorithm(Config cfg);

/**
 * Based on the configuration, get a function pointer for algorithm.
 */
algo_func_t get_algo_func(Config cfg, size_t input_size, obj_map_t& obj_map);

/**
 * Builds up the mapping of identifier (used in config) to objective function pointer.
 */
obj_map_t create_obj_map();

/**
 * Builds up the mapping of identifier (used in config) to algorithm function pointer.
 */
algo_map_t create_algo_map();
