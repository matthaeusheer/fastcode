#include <vector>
#include <iostream>
#include <functional>

#include "tsc_x86.h"
#include "cpp_utils.h"
#include "benchmark.h"

#include "hgwosca.h"
#include "penguin.h"
#include "pso.h"
#include "squirrel.h"


std::vector<timeInt64> time_algorithm(Config cfg) {

  // First creating function name to function pointer maps
  auto obj_func_map = create_obj_map();
  auto algo_func_map = create_algo_map();

  if (obj_func_map.find(cfg.obj_func) == obj_func_map.end()) {
    throw std::invalid_argument("There is no registered objective function called " + cfg.obj_func);
  }

  if (algo_func_map.find(cfg.algorithm) == algo_func_map.end()) {
    throw std::invalid_argument("There is no registered algorithm called " + cfg.algorithm);
  }

  // Bind the parameters such that we have one generic algorithm function to run and benchmark
  auto algo_func = std::bind(algo_func_map[cfg.algorithm],
                             obj_func_map[cfg.obj_func],
                             cfg.population,
                             cfg.dimension,
                             cfg.n_iterations,
                             cfg.min_position,
                             cfg.max_position);

  std::vector<timeInt64> cycles_vec;
  double *solution;

  // Run the actual algorithm and time it for n_iterations
  for (int rep = 0; rep < cfg.n_repetitions; ++rep) {
    timeInt64 start_time = start_tsc();

    solution = algo_func();

    timeInt64 cycles = stop_tsc(start_time);

    cycles_vec.emplace_back(cycles);

    #ifdef DEBUG
        // Store final solution values per repetition
        std::string file_path = add_str_before_file_end(cfg.solution_file, "_rep_" + std::to_string(rep));
        store_solutions(solution, cfg.dimension, file_path);

        // Store objective function
        // So far no possibility to get the objective value here...
    #endif
  }

  free(solution);

  return cycles_vec;
}


obj_map_t create_obj_map() {

  // Register more objective functions here as they get implemented.
  obj_map_t obj_map = {{"sum_of_squares", &sum_of_squares},
                       {"sum",            &sum},
                       {"sum_negative",   &sum_negative},
                       {"rastigrin",      &rastigrin},
                       {"rosenbrock",     &rosenbrock},
                       {"rosenbrock_neg", &rosenbrock_neg},
                       {"sphere",         &sphere},
                       {"egghol2d",       &egghol2d},
                       {"schaf2d",        &schaf2d},
                       {"griewank",       &griewank}};
  return obj_map;
}


algo_map_t create_algo_map() {

  // Register more algorithms here as they get implemented.
  algo_map_t algo_map = {{"hgwosca",  &gwo_hgwosca},
                         {"penguin",  &pen_emperor_penguin},
                         {"pso",      &pso_basic},
                         {"squirrel", &squirrel}};

  return algo_map;
}
