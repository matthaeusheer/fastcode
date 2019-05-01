#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "objectives.h"

#include <criterion/criterion.h>

/*
  Testing the sum of squares
*/
Test(obj_unit, sum_of_squares) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum_of_squares(args, 6),  - 55.0,  FLT_EPSILON, "sum of squares function works as expected.");
}

/*
  Testing the sum
*/
Test(obj_unit, sum) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum(args, 6), 15.0, FLT_EPSILON, "sum function should work as expected.");
}

/*
  Testing the Rastingrin function
*/
Test(obj_unit, rastigrin) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(rastigrin(args, 6), -50, FLT_EPSILON, "rastigrin function works as expected.");
}

/*
  Testing Multidimensional Rosenbrock Function
*/
Test(obj_unit, rosenbrock) {
  float args[] = {0, 1, 2, 3, 4, 5, 6};
  cr_expect_float_eq(rosenbrock(args, 7), - 51031, FLT_EPSILON, "rosenbrock function works as expected.");
}

/*
  Testing multidimensional sphere
*/
Test(obj_unit, sphere) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sphere(args, 6), - 55, FLT_EPSILON, "sphere function works as expected.");
}

/*2D Schaffer Function
  Testing
*/
Test(obj_unit, schaf2d) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(schaf2d(args, 6), - 0.7077, FLT_EPSILON, "schaf2d function works as expected.");
}

/*
  Testing Griewank function
*/
Test(obj_unit, griewank) {
  float args[] = {1, 2, 3, 4, 5};
  cr_expect_float_eq(griewank(args, 5), - 1.0172 , FLT_EPSILON, "griewank function works as expected.");
}

/*
 *Testing Schwefel 1
 */
Test(obj_unit, schwefel01) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(schwefel01(args, 6), - 293434.139229145, FLT_EPSILON, "schwefel01 function works as expected.");
}


/*
 *Testing cosmix
 */
Test(obj_unit, cosinemixture) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(cosinemixture(args, 6), - 55, FLT_EPSILON, "cosinemixture function works as expected.");
}

/*
 *Testing powel
 */
Test(obj_unit, powel) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(powel(args, 4), - 50, FLT_EPSILON, "powel function works as expected.");
}

/*
 *Testing freundschtein and roth
 */
Test(obj_unit, freundsteinroth) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(freundsteinroth(args, 4), - 1802, FLT_EPSILON, "freundschtein and roth function works as expected.");
}

/*
 *Testing Beale
 */

Test(obj_unit, beale) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(beale(args, 3), - 14.2031, FLT_EPSILON, "Beale function works as expected.");
}
