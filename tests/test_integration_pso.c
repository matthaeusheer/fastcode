#include <criterion/criterion.h>

#include "testing_utilities.h"
#include "objectives.h"
#include "pso.h"

#define SWARM_SIZE 1000
#define DIM 2


Test(pso_integration, sum_of_squares) {
  test_algo(sum_of_squares, SWARM_SIZE, DIM, -10, 10, 1000, pso_basic, 0, 0.1,
            true, "PSO", "sum_of_squares");
}

Test(pso_integration, sphere) {
  test_algo(sphere, SWARM_SIZE, DIM, -10, 10, 1000, pso_basic, 0, 0.1,
            true, "PSO", "sphere");
}

Test(pso_integration, rosenbrock) {
  test_algo(rosenbrock, SWARM_SIZE, DIM, -30, 30, 1000, pso_basic, 0, 0.01,
            true, "PSO", "rosenbrock");
}
