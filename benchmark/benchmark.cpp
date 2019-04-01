#include <vector>
#include <iostream>
#include <utility>
#include <string>
#include <cstdlib>

#include "utils.h"
#include "benchmark.h"
#include "range.h"
#include "tsc_x86.h"
#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"


std::vector<timeInt64> time_algorithm(Config cfg) {

    // First creating function name to function pointer maps
    auto obj_func_map = create_obj_map();
    auto algo_func_map = create_algo_map();

    double* mins = filled_array((size_t)cfg.dimension, cfg.min_position);
    double* maxs = filled_array((size_t)cfg.dimension, cfg.max_position);

    // Bind the parameters such that we have one generic algorithm function to run and benchmark
    auto algo_func = std::bind(algo_func_map[cfg.algorithm],
                               obj_func_map[cfg.obj_func],
                               cfg.population,
                               cfg.dimension,
                               cfg.n_iterations,
                               mins,
                               maxs);

    std::vector<timeInt64> cycles_vec;
    double* solution;

    // Run the actual algorithm and time it for n_iterations
    for (int iter = 0; iter < cfg.n_iterations; ++iter) {
        timeInt64 start_time = start_tsc();

        solution = algo_func();

        timeInt64 cycles = stop_tsc(start_time);

        cycles_vec.emplace_back(cycles);

        // TODO: Add DEBUG flag to print solutions and timings.
        // print_solution((size_t)cfg.dimension, solution);
        // std::cout << " Cycles: " << cycles << std::endl;
    }

    // TODO: We might want to be able to store all solutions over all runs to see whether they are consistent.

    store_solutions(solution, cfg.dimension, cfg.solution_file);  // Store solution of last run

    free(solution);
    free(mins);
    free(maxs);

    return cycles_vec;
}


obj_map_t create_obj_map() {

    // Register more objective functions here as they get implemented.
    obj_map_t obj_map = {{"sum_of_squares",  &sum_of_squares},
                         {"sum",             &sum},
                         {"rastigrin",       &rastigrin},
                         {"rosenbrock",      &rosenbrock},
                         {"sphere",          &sphere},
                         {"egghol2d",        &egghol2d},
                         {"schaf2d",         &schaf2d},
                         {"griewank",        &griewank}};
    return obj_map;
}


algo_map_t create_algo_map() {

    // Register more algorithms here as they get implemented.
    algo_map_t algo_map = {{"hgwosca", &hgwosca}};

    return algo_map;
}
