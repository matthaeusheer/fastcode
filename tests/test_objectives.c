#include <float.h>
#include <math.h>
#include "objectives.h"

#include <criterion/criterion.h>

Test(obj_unit, sum) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum(args, 6), 15.0, FLT_EPSILON, "sum function should work as expected.");
}
