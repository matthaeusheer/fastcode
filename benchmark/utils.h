#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>

#include <stdbool.h>


/**
   Command line arguments container.
*/
typedef struct {
    std::string algorithm;
    std::string obj_func;
    std::string out_file;
    std::string solution_file;
    int dimension;
    int population;
    int n_iterations;
    int min_position;
    int max_position;
    bool verbose;
} Config;

/**
 *  Parses command line arguments and stores them in a Config type pointer.
 */
void parse_args(Config* args, int argc, char* argv[]);

/**
 *  Prints the content of a Config type struct.
 */
void print_config(Config config);

/**
 *  Prints the output of any file line by line.
 */
void print_file(std::string file_path);

// TODO: Checks if they are actually defined.
/**
 *  Prints GIT_VERSION, GIT_COMMIT, GIT_DATE environment variables to console.
 */
void print_git_version();

/**
 *  Writes measured timings (stored in a vector for all iterations) to a specified file.
 */
void store_timings(const std::vector<unsigned long long>& cycles_vec, std::string file_path);

/**
 *  Writes the solution array of one algorithm output to a specified file.
 */
void store_solutions(double* solution, int dimension, std::string file_path);

/**
 *  Prints the solution array of one algorithm output to console.
 */
void print_solution(size_t dim, const double* const solution);

/**
 * Fills an double array with a given length and unique value for all entries.
 */
double* filled_array(size_t length, double val);
