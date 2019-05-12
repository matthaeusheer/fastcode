#include <criterion/criterion.h>

#include "testing_utilities.h"
#include "objectives.h"
#include "hgwosca.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


Test(hgwosca_integration, sum) {
  test_algo(sum, 30, 10, -100, 100, 100, gwo_hgwosca, -100 * 10, 0.2 * 10,
            true, "HGWOSCA", "sum");
}

Test(hgwosca_integration, sum_2) {
  test_algo(sum, 50, 2, -100, 100, 100, gwo_hgwosca, -100 * 2, 0.2 * 2,
            true, "HGWOSCA", "sum_2");
}

Test(hgwosca_integration, rosenbrock) {
  test_algo(rosenbrock, 40, 3, -100, 100, 1500, gwo_hgwosca, 0, 0.001,
            true, "HGWOSCA", "rosenbrock");
}

Test(hgwosca_integration, sphere) {
  test_algo(sphere, 30, 10, -100, 100, 800, gwo_hgwosca, 0, 0.5,
            true, "HGWOSCA", "sphere");
}
