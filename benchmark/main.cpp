#include <float.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

#include "run_algos.h"
#include "utils.h"
#include "benchmark.h"
#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"

#define FILE_NAME "output.txt"


int main(int argc, const char** argv) {

    if (argc != 2) {
        std::cout << "Usage: config file name." << std::endl;
        exit(1);
    }

    /*************************************
        Running algorithms
    *************************************/

    auto config = parse_config(std::string(argv[1]));
    auto timings = time_algorithm(config);
    auto input_sizes = timings.first;
    auto timed_cycles = timings.second;

    /*************************************
        Storing results
    *************************************/

    std::ofstream outfile;
    outfile.open(FILE_NAME);

    outfile << "# Git Has"  << std::endl;
    outfile << "git version - " << GIT_VERSION << std::endl;
    outfile << "git commit - " << GIT_COMMIT << std::endl;
    outfile << "git date - " << GIT_DATE << std::endl;

    outfile << "\n# Parameters"  << std::endl;
    for (auto const& param_pair : config)
    {
        outfile << param_pair.first
                << ':'
                << param_pair.second
                << std::endl ;
    }

    outfile << "\n# Timings" << std::endl;

    for (int i = 0; i < timed_cycles.size(); ++i) {

        outfile << "Input size: " << input_sizes[i] << ", "
                << "Runtime: " << timed_cycles[i] << " cycles." << std::endl;
    }

    outfile.close();

    /*************************************
        Print complete output file
    *************************************/
    std::cout << "\nWritten output file..." << std::endl;
    print_file(FILE_NAME);

    return 0;
}
