#include <float.h>
#include <math.h>
#include "objectives.h"

#include <criterion/criterion.h>

Test(obj_unit, sum) {
    double args[] = {0, 1, 2, 3, 4, 5};
    cr_assert(abs(sum(args, 6) - 15.0) < DBL_EPSILON, "sum function works as expected.");
}
