#include <criterion/criterion.h>

#include "utils_test.h"
#include "objectives.h"
#include "penguin.h"


Test(pengu_integration, sum) {
  test_algo(sum, 50, 2, -5, 5, 100, pen_emperor_penguin, -10, 0.1, false);
}

Test(pengu_integration, sum_of_squares) {
  test_algo(sum_of_squares, 50, 2, -5, 5, 100, pen_emperor_penguin, 0, 0.1, false);
}

Test(pengu_integration, rosenbrock) {
  test_algo(rosenbrock, 100, 2, -5, 5, 200, pen_emperor_penguin, 0, 1, false);
}

Test(pengu_integration, rastigrin) {
  test_algo(rastigrin, 100, 4, -5, 5, 200, pen_emperor_penguin, 0, 0.1, false);
}

Test(pengu_integration, sphere) {
  test_algo(sphere, 100, 2, -5, 5, 200, pen_emperor_penguin, 0, 0.1, false);
}
