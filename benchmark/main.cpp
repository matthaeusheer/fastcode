#include <float.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "run_algos.h"
#include "utils.h"
#include "benchmark.h"
#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"


int main(int argc, const char** argv) {

    if (argc != 2) {
        std::cout << "Usage: config file name." << std::endl;
        exit(1);
    }

    printf("GIT VERSION - %s\n", GIT_VERSION);
    printf("GIT COMMIT  - %s\n", GIT_COMMIT);
    printf("GIT DATE    - %s\n", GIT_DATE);


    auto config = parse_config(std::string(argv[1]));

    time_algorithm(config);

    exit(0);
    /*******************************************************************************
        RUN HGOWOSCA ALGORITHM
    *******************************************************************************/
    /*
    constexpr int min_dim = 100;
    constexpr int max_dim = 1000;
    constexpr int step_dim = 100;

    std::vector<int> timed_cycles;
    std::vector<int> input_sizes;

    for(auto const& dim: range<int, min_dim, max_dim, step_dim>()) {

        myInt64 start_time = start_tsc();
        run_hgwosca(sum_of_squares, 30, dim, 120, -100, 100, false);
        myInt64 cycles = stop_tsc(start_time);

        timed_cycles.emplace_back(cycles);
        input_sizes.emplace_back(dim);
    }

    std::ofstream outfile;
    outfile.open("HGOWOSCA_timings.csv");

    for (int i = 0; i < timed_cycles.size(); ++i) {
        std::cout << "Input size: " << input_sizes[i] << ", "
                  << "Runtime: " << timed_cycles[i] << " cycles." << std::endl;

        outfile << "Input size: " << input_sizes[i] << ", "
                << "Runtime: " << timed_cycles[i] << " cycles." << std::endl;
    }

    outfile.close();
    */
    /*******************************************************************************
        RUN PINGU ALGORITHM
    *******************************************************************************/

    // Coming soon.


    /*******************************************************************************
        RUN PCO ALGORITHM
    *******************************************************************************/

    // Coming soon.


    /*******************************************************************************
        RUN PCO FANCY ALGORITHM
    *******************************************************************************/

    // Coming soon.

}
