#include <vector>
#include <iostream>
#include <utility>

#include "utils.h"
#include "run_algos.h"
#include "tsc_x86.h"
#include "benchmark.h"
#include "range.h"


std::pair<std::vector<int>, std::vector<int>> time_algorithm(config_t config) {

    auto obj_func_map = create_obj_map();

    int min_dim = std::stoi(config["min_dim"]);
    int max_dim = std::stoi(config["max_dim"]);
    int step_dim = std::stoi(config["step_dim"]);

    std::vector<int> timed_cycles;
    std::vector<int> input_sizes;

    for (auto input_size : whoshuu::range(min_dim, max_dim + step_dim, step_dim)) {

        // std::cout << "running for size " << input_size << std::endl;

        auto algo_func = get_algo_func(config, input_size, obj_func_map);

        myInt64 start_time = start_tsc();

        /*************************************
            Running algorithm
        *************************************/

        algo_func();

        /*************************************

        *************************************/

        myInt64 cycles = stop_tsc(start_time);

        timed_cycles.emplace_back(cycles);
        input_sizes.emplace_back(input_size);
    }

    return std::pair<std::vector<int>, std::vector<int>> (input_sizes, timed_cycles);


}


algo_func_t get_algo_func(config_t cfg, size_t input_size, obj_map_t& obj_map) {

    algo_func_t algo_func;

    if (cfg["algorithm"] == "GWO") {
        return std::bind(run_hgwosca,
                         obj_map[cfg["obj_func"]],
                         std::stoi(cfg["wolf_count"]),
                         input_size,
                         std::stoi(cfg["max_iterations"]),
                         std::stoi(cfg["min_position"]),
                         std::stoi(cfg["max_position"]),
                         false);

    } else if (cfg["algorithm"] == "pingu") {
        std::cout << "Pingu algorithm (" << cfg["algorithm"] << ") not yet implemented." << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Configured algorithm (" << cfg["algorithm"] << ") unknown." << std::endl;
        exit(EXIT_FAILURE);
    }

    return algo_func;
}

