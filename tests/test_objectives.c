#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "objectives.h"
#include "utils.h"

#include <criterion/criterion.h>

/*
  Testing the sum of squares
*/
Test(obj_unit, sum_of_squares) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum_of_squares(args, 6), 55.0, FLT_EPSILON,
                     "sum_of_squares should work on small dimensions");

  double *args_2 = filled_double_array(100, 2.0);
  cr_expect_float_eq(sum_of_squares(args_2, 100), 400.0, FLT_EPSILON,
                     "sum_of_squares should work on larger dimensions");
}

/*
  Testing the sum
*/
Test(obj_unit, sum) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum(args, 6), 15.0, FLT_EPSILON,
                     "sum function should work as expected");
}

/*
  Testing the Rastingrin function
*/
Test(obj_unit, rastigrin) {
  double args[] = {0.0, 0.0};
  cr_expect_float_eq(rastigrin(args, 2), 0.0, FLT_EPSILON, "rastigrin function works as expected.");
}

/*
  Testing Multidimensional Rosenbrock Function
*/
Test(obj_unit, rosenbrock) {
  double args[] = {1, 1, 1};
  cr_expect_float_eq(rosenbrock(args,3), 0.0, FLT_EPSILON, "rosenbrock function works as expected.");
}

/*
  Testing multidimensional sphere
*/
Test(obj_unit, sphere) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sphere(args, 6), 55.0, FLT_EPSILON, "sphere function works as expected.");
}

/*2D Schaffer Function
  Testing
*/
Test(obj_unit, schaf2d) {
  double args[] = {0.0, 0.0};
  cr_expect_float_eq(schaf2d(args, 2), 0.0, FLT_EPSILON, "schaf2d function works as expected.");
}


/*
  Testing Griewank function
*/
Test(obj_unit, griewank) {
  double args[] = {0.0, 0.0, 0.0, 0.0};
  cr_expect_float_eq(griewank(args, 4), 0.0 , FLT_EPSILON, "griewank function works as expected.");
}

/*
 *Testing Schwefel 1
 */
Test(obj_unit, schwefel01) {
  double args[] = {0.0, 0.0, 0.0, 0.0};
  cr_expect_float_eq(schwefel01(args, 4), 0.0, FLT_EPSILON, "schwefel01 function works as expected.");
}


/*
 *Testing powel
 */
Test(obj_unit, powel) {
  double args[] = {0, 0, 0, 0};
  cr_expect_float_eq(powel(args, 4), 0.0, FLT_EPSILON, "powel function works as expected.");
}

/*
 *Testing freundschtein and roth
 */
Test(obj_unit, freundsteinroth) {
  double args[] = {5.0, 4.0};
  cr_expect_float_eq(freundsteinroth(args, 2), 0.0, FLT_EPSILON, "freundschtein and roth function works as expected.");
}

/*
 *Testing Beale
 */

Test(obj_unit, beale) {
  double args[] = {3.0, 0.5};
  cr_expect_float_eq(beale(args, 2), 0, FLT_EPSILON, "Beale function works as expected.");
}
