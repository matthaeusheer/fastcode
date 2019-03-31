#pragma once

#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <stdbool.h>


std::map<std::string, std::string> parse_config(const std::string& file_name);

/**
   Generate a random number between 0 and 1.
*/
double randf();

/**
   Build an array of size `ARRAY_SIZE` and fill it with random numbers.
*/
double* build();

/**
   Command line arguments.
   Currently not really needed but could become more useful in the future.
*/
typedef struct {
    bool verbose;
    char* algorithm;
    int n;
} Clargs;

/**
 * Open a file and print its content line by line.
 */
void print_file(std::string file_path);