#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "objectives.h"
#include "utils.h"
#include "pso.h"

#include <criterion/criterion.h>



/*
  Testing the sum of squares
*/
Test(obj_unit, sum_of_squares) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum_of_squares(args, 6), 55.0, FLT_EPSILON,
                     "sum_of_squares should work on small dimensions");

  float *args_2 = filled_float_array(100, 2.0);
  cr_expect_float_eq(sum_of_squares(args_2, 100), 400.0, FLT_EPSILON,
                     "sum_of_squares should work on larger dimensions");
}

/*
  Testing the sum of squares
*/
Test(obj_unit, simd_sum_of_squares) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(simd_sum_of_squares(args, 6), 55.0, FLT_EPSILON,
                     "sum_of_squares should work on small dimensions");

  float *args_2 = filled_float_array(100, 2.0);
  cr_expect_float_eq(simd_sum_of_squares(args_2, 100), 400.0, FLT_EPSILON,
                     "sum_of_squares should work on larger dimensions");
}



/*
  Testing the sum
*/
Test(obj_unit, sum) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sum(args, 6), 15.0, FLT_EPSILON,
                     "sum function should work as expected");
}

/*
  Testing the Rastingrin function
*/
Test(obj_unit, rastigrin) {
  float args[] = {0.0, 0.0};
  cr_expect_float_eq(rastigrin(args, 2), 0.0, FLT_EPSILON, "rastigrin function works as expected.");
}

/*
  Testing Multidimensional Rosenbrock Function
*/
Test(obj_unit, rosenbrock) {
  float args[] = {1, 2, 3};
  cr_expect_float_eq(rosenbrock(args,3), 201.0, FLT_EPSILON, "rosenbrock function works as expected.");
  // printf("%f\n", rosenbrock(args,3) );
}

/*
  Testing Multidimensional Rosenbrock Function
*/
Test(obj_unit, simd_rosenbrock) {
  float args[] = {2, 1, 1, 1000000, 1 , 5, 2, 1, 1, 1 , 2 , 12 , 13321 , 546 , 446 , 656, 64  , 654 , 654};

  cr_expect_float_eq(simd_rosenbrock(args,18), rosenbrock(args,18), FLT_EPSILON, "simd_rosenbrock function works as expected.");
}

/*
  Testing Multidimensional Rosenbrock Function
*/
Test(obj_unit, opt_simd_rosenbrock) {
  cr_skip_test();
  float args[] = {2, 1, 1, 1000000, 1 , 5, 2, 1, 1, 1 , 2 , 12 , 13321 , 546 , 446 , 656, 64};
  __m256 simd_args[] = {_mm256_loadu_ps(args), _mm256_loadu_ps(&args[8])};
  cr_expect_float_eq(opt_simd_rosenbrock(simd_args,2), rosenbrock(args,16), FLT_EPSILON, "simd_rosenbrock function works as expected.");
}

/*
  Testing multidimensional sphere
*/
Test(obj_unit, sphere) {
  float args[] = {0, 1, 2, 3, 4, 5};
  cr_expect_float_eq(sphere(args, 6), 55.0, FLT_EPSILON, "sphere function works as expected.");
}

/*2D Schaffer Function
  Testing
*/
Test(obj_unit, schaf2d) {
  float args[] = {0.0, 0.0};
  cr_expect_float_eq(schaf2d(args, 2), 0.0, FLT_EPSILON, "schaf2d function works as expected.");
}


/*
  Testing Griewank function
*/
Test(obj_unit, griewank) {
  float args[] = {0.0, 0.0, 0.0, 0.0};
  cr_expect_float_eq(griewank(args, 4), 0.0 , FLT_EPSILON, "griewank function works as expected.");
}

/*
 *Testing Schwefel 1
 */
Test(obj_unit, schwefel01) {
  float args[] = {0.0, 0.0, 0.0, 0.0};
  cr_expect_float_eq(schwefel01(args, 4), 0.0, FLT_EPSILON, "schwefel01 function works as expected.");
}


/*
 *Testing powel
 */
Test(obj_unit, powel) {
  float args[] = {0, 0, 0, 0};
  cr_expect_float_eq(powel(args, 4), 0.0, FLT_EPSILON, "powel function works as expected.");
}

/*
 *Testing freundschtein and roth
 */
Test(obj_unit, freundsteinroth) {
  float args[] = {5.0, 4.0};
  cr_expect_float_eq(freundsteinroth(args, 2), 0.0, FLT_EPSILON, "freundschtein and roth function works as expected.");
}

/*
 *Testing Beale
 */

Test(obj_unit, beale) {
  float args[] = {3.0, 0.5};
  cr_expect_float_eq(beale(args, 2), 0, FLT_EPSILON, "Beale function works as expected.");
}
