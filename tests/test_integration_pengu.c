#include <criterion/criterion.h>

#include "testing_utilities.h"
#include "objectives.h"
#include "penguin.h"


Test(pengu_integration, sum) {
  test_algo(sum, 50, 2, -5, 5, 50, pen_emperor_penguin, -10, 0.1,
            true, "EPC", "sum");
}

Test(pengu_integration, sum_of_squares) {
  test_algo(sum_of_squares, 50, 2, -5, 5, 50, pen_emperor_penguin, 0, 0.1,
            true, "EPC", "sum_of_squares");
}

Test(pengu_integration, rosenbrock) {
  test_algo(rosenbrock, 500, 2, -5, 5, 2000, pen_emperor_penguin, 0, 0.3,
            true, "EPC", "rosenbrock");
}

Test(pengu_integration, rastigrin) {
  test_algo(rastigrin, 100, 2, -5, 5, 2000, pen_emperor_penguin, 0, 0.1,
            true, "EPC", "rastigrin");
}

Test(pengu_integration, sphere) {
  test_algo(sphere, 100, 2, -5, 5, 200, pen_emperor_penguin, 0, 0.1,
            true, "EPC", "sphere");
}
