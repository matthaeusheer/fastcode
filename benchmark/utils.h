#pragma once

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

#include <stdbool.h>


/**
   Generate a random number between 0 and 1.
*/
double randf();

/**
   Build an array of size `ARRAY_SIZE` and fill it with random numbers.
*/
double* build();

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


void parse_args(Config* args, int argc, char* argv[]);

void print_file(std::string file_path);

void print_git_version();

void print_config(Config config);

void store_timings(const std::vector<unsigned long long>& cycles_vec, std::string file_path);

void store_solutions(double* solution, int dimension, std::string file_path);