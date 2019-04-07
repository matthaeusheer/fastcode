
#include <stdio.h>
#include <iostream>
#include <vector>

#include "cpp_utils.h"
#include "benchmark.h"


int main(int argc, char *argv[]) {

  Config config;
  parse_args(&config, argc, argv);
  print_config(config);

  auto cycles_vec = time_algorithm(config);

  store_timings(cycles_vec, config.out_file);

  return 0;
}
