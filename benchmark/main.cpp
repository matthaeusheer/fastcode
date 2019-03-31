#include <float.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "run_algos.h"
#include "../objectives/objectives.h"
#include "../operation-bench/tsc_x86.h"
#include "utils.hpp"


int main() {

    /*******************************************************************************
        RUN HGOWOSCA ALGORITHM
    *******************************************************************************/

    // TODO: All parameters should be configurable either from cmd or with a json file.
    // Even better: make algorithm and objective function parameters as well.
    // Such we have a tool to always benchmark a certain
    // algo with a certain objective function (or multiple) and a parameter set for the algo.
    // The tool should should validate params with checks and an evaluation tool should
    // then automatically plot this data.
    // The timing data file should hold all this metadata as well as additional code
    // metadata such as branch and last git commit ID.

    int min_dim = 100;
    int max_dim = 1000;
    int step_dim = 100;

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
