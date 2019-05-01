#include <criterion/criterion.h>

#include "utils_test.h"
#include "objectives.h"
#include "squirrel.h"

#define POPULATION 10000
#define DIM 2


Test(squirrel_integration, sum_of_squares) {
  test_algo(sum_of_squares, POPULATION, DIM, -10, 10, 250, squirrel, 0, 0.1, false);
}

Test(squirrel_integration, rosenbrock) {
  test_algo(rosenbrock, POPULATION, DIM, -30, 30, 1000, squirrel, 0, 0.1, false);
}

Test(squirrel_integration, sphere) {
  test_algo(sphere, POPULATION, DIM, -5, 5, 500, squirrel, 0, 0.1, false);
}
