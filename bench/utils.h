#pragma once

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
   Command line arguments.
   Currently not really needed but could become more useful in the future.
*/
typedef struct {
  bool verbose;
  char* algorithm;
  int n;
} Clargs;

/**
   Parse command line arguments.
*/
void parse_args(Clargs* args, int argc, char* argv[]);
