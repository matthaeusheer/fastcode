#include <criterion/criterion.h>

#include "testing_utilities.h"
#include "objectives.h"
#include "pso.h"

#define SWARM_SIZE 1024
#define DIM 8

Test(pso_integration, opt_simd_sum_of_squares) {
  test_simd_algo(opt_simd_sum_of_squares, SWARM_SIZE, DIM, -10, 10, 6000, pso_basic, 0, 0.1,
            true, "PSO", "sum_of_squares");
}


Test(pso_integration, opt_simd_rosenbrock) {
  test_simd_algo(opt_simd_rosenbrock, 6000, DIM, -30, 30, 3000, pso_basic, 0, 6,
            true, "PSO", "rosenbrock");
}
