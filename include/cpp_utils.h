#pragma once

#include <string>
#include <vector>

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
    int n_iterations;  // iterations inside the algorithm
    int n_repetitions;  // repetitions for average timing
    int min_position;
    int max_position;
    bool verbose;
} Config;

/**
 *  Parses command line arguments and stores them in a Config type pointer.
 */
void parse_args(Config *args, int argc, char *argv[]);

/**
 *  Prints the content of a Config type struct.
 */
void print_config(Config config);

/**
 *  Writes measured timings (stored in a vector for all iterations) to a specified file.
 */
void store_timings(const std::vector<unsigned long long> &cycles_vec, std::string file_path);

/**
 *  Writes the solution array of one algorithm output to a specified file.
 */
void store_solutions(double *solution, int dimension, std::string file_path);

/**
 * Splits an input string (e.g. output_file.txt) to a vector of {output_file, txt) in case of character '.'
 */
std::vector<std::string> split_str(const std::string &s, char c);

/**
 * Inserts some substring directly in front of a file ending.
 * Example: add_str_before_file_end("out_file.txt", "_3") results in "out_file_3.txt"
 */
std::string add_str_before_file_end(std::string file_path, std::string insert);

/**
 *  Prints the output of any file line by line.
 */
void print_file(std::string file_path);


// TODO: Checks if they are actually defined.
/**
 *  Prints GIT_VERSION, GIT_COMMIT, GIT_DATE environment variables to console.
 */
void print_git_version();
