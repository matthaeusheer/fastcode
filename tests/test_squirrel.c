/*****************************
BASIC SQUIRREL UNIT TESTS
*****************************/

#include <float.h>
#include <math.h>

#include "objectives.h"
#include "squirrel.h"
#include "utils.h"

#include <criterion/criterion.h>

#ifndef SQRT_PI
#define SQRT_PI 1.77245385091
#endif

Test(squirrel_unit,sqr_eval_fitness){
  size_t pop_size, dim;
  pop_size = 4;
  dim = 2;
  float x[] = {
    0.0,  0.0,
    2.5,  4.0,
   10.0,  3.0,
  -10.0, -3.0
  };
  float fitness[pop_size];
  sqr_eval_fitness(sum_of_squares, pop_size, dim, x, fitness);
  cr_expect_float_eq(fitness[0], 0.0, FLT_EPSILON,
                     "first particle fitness should be 0.0");
  cr_expect_float_eq(fitness[1], 22.25, FLT_EPSILON,
                     "second particle fitness should be 22.5");
  cr_expect_float_eq(fitness[2], 109.0, FLT_EPSILON,
                     "third particle's fitness should be 109");
  cr_expect_float_eq(fitness[3], 109.0, FLT_EPSILON,
                     "fourth particle's fitness should be 109");
}

Test(squirrel_unit,sqr_lowest_val_to_front){
  float x[] = { 12.5, 133.0,  3.0, -133.0, 133.01 };
  float y[] = {
    0, 0,
    1, 1,
    2, 2,
    3, 3,
    4, 4
  };

  sqr_lowest_val_to_front(x,y,5,2);
  cr_expect_float_eq(x[0], -133.0, FLT_EPSILON, "best fitness should be 3.0");
  cr_expect_float_eq(x[3], 12.5, FLT_EPSILON,
                     "replaced fitness from 0th element");
  cr_expect_float_eq(y[0], 3, FLT_EPSILON, "best fitness at 3,3");
  cr_expect_float_eq(y[1], 3, FLT_EPSILON, "best fitness at 3,3");
  cr_expect_float_eq(y[6], 0, FLT_EPSILON, "replaced fitness at 0,0");
  cr_expect_float_eq(y[7], 0, FLT_EPSILON, "replaced fitness at 0,0");
}

Test(squirrel_unit,sqr_lowest4_vals_to_front){
  float x[] = { 133.0, 12.5, 3.0, -133.0, -133.01 };
  float y[] = {
    0, 0,
    1, 1,
    2, 2,
    3, 3,
    4, 4
  };

  sqr_lowest4_vals_to_front(x,y,5,2);

  cr_expect_float_eq(x[0], -133.01, 1e-3, "best fitness should be 3.0");
  cr_expect_float_eq(x[1], -133.0, FLT_EPSILON,
                     "best fitness replaced by 3rd element");
  cr_expect_float_eq(x[2], 3.0, FLT_EPSILON, "best fitness should be 3.0");
  cr_expect_float_eq(x[3], 12.5, FLT_EPSILON,
                     "best fitness replaced by 3rd element");
  cr_expect_float_eq(x[4], 133.0, FLT_EPSILON,
                     "best fitness replaced by 3rd element");
  cr_expect_float_eq(y[0], 4, FLT_EPSILON, "best fitness at 4,4");
  cr_expect_float_eq(y[1], 4, FLT_EPSILON, "best fitness at 4,4");
  cr_expect_float_eq(y[2], 3, FLT_EPSILON, "2nd best fitness at 3,3");
  cr_expect_float_eq(y[3], 3, FLT_EPSILON, "2nd best fitness at 3,3");
  cr_expect_float_eq(y[4], 2, FLT_EPSILON, "3rd best fitness at 2,2");
  cr_expect_float_eq(y[5], 2, FLT_EPSILON, "3rd best fitness at 2,2");
  cr_expect_float_eq(y[6], 1, FLT_EPSILON, "4th best fitness at 1,1");
  cr_expect_float_eq(y[7], 1, FLT_EPSILON, "4th best fitness at 1,0");
  cr_expect_float_eq(y[8], 0, FLT_EPSILON, "worst best fitness at 0,0");
  cr_expect_float_eq(y[9], 0, FLT_EPSILON, "worst best fitness at 0,0");
}

Test(squirrel_unit, seasonal_const){
  size_t dim = 2;
  float y[] = {
    10, 2,
    1.5, 2.5,
    4., 3.,
    5.5, 5,
    4.0, 4.5
  };

  float sc = sqr_eval_seasonal_cons(y,dim);
  cr_expect_float_eq(sc, 11.77921898938974646313234009332, 1e-3,
                     "the value of seasonal costant must be 11.7792");
}

Test(squirrel_unit,eval_smin){
  float val0 = sqr_eval_smin(0); // = 1e-6
  float val1 = sqr_eval_smin(8); // = 1e-6/365

  cr_expect_float_eq(val0, 1e-6, FLT_EPSILON,
                     "highest possible value at the start of the process");
  cr_expect_float_eq(val1, 2.739726027397260273972602739726e-9, 1e-6,
                     "for iter = 8, the exponents cancel out");
}

Test(squirrel_unit, factorial){
  float fac1 = sqr_factorial(2);
  float fac2 = sqr_factorial(3);
  float fac3 = sqr_factorial(0);
  float fac4 = sqr_factorial(10);

  cr_expect_eq(fac1, 2, "2! = 2");
  cr_expect_eq(fac2, 6, "3! = 6");
  cr_expect_eq(fac3, 1, "0! = 1");
  cr_expect_eq(fac4, 3628800, "10! = 3628800");
}


Test(squirrel_unit, gamma_function){
  float val1 = sqr_eval_gamma(0.5);
  float val3 = sqr_eval_gamma(1.5);
  float val5 = sqr_eval_gamma(2.5);
  float val6 = sqr_eval_gamma(3.5);
  float val7 = sqr_eval_gamma(-2.5);

  cr_expect_float_eq(val1, SQRT_PI , FLT_EPSILON, " gamma(0.5) = 1.722453");
  cr_expect_float_eq(val3, SQRT_PI/2, FLT_EPSILON, "gamma(1.5) = 0.886226 ");
  cr_expect_float_eq(val5, 3*SQRT_PI/4, FLT_EPSILON, "gamma(2.5 ) = 1.329340");
  cr_expect_float_eq(val6, 15*SQRT_PI/8, FLT_EPSILON, " gamma(3.5) = 3.323350 ");
  cr_expect_float_eq(val7, -8*SQRT_PI/15, FLT_EPSILON, " gamma(-2.5) = 0.945 ");
}
