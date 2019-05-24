#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "float.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(utils_unit, random_0_to_1) {
  for(size_t iter = 0; iter < 20; iter++) {
    double r = random_0_to_1();
    cr_expect_leq(r, 1, "random_0_to_1 should be bound above by 1");
    cr_expect_geq(r, 0, "random_0_to_1 should be bound below by 0");
  }
}


Test(utils_unit, linear_scale) {
  double start = 1.0;
  double end = 10.0;
  size_t iter_count = 9;
  cr_expect_float_eq(linear_scale(start, end, iter_count, 0), 1.0, FLT_EPSILON,
                     "iteration 0 should be at start");
  cr_expect_float_eq(linear_scale(start, end, iter_count, 2), 3.0, FLT_EPSILON,
                     "iteration 2 should be scaled");
  cr_expect_float_eq(linear_scale(start, end, iter_count, 9), 10.0, FLT_EPSILON,
                     "iteration 9 should be at end");

  // start > end
  start = 10.0;
  end = 1.0;
  iter_count = 9;
  cr_expect_float_eq(linear_scale(start, end, iter_count, 0), 10.0, FLT_EPSILON,
                     "iteration 0 should be at start");
  cr_expect_float_eq(linear_scale(start, end, iter_count, 2), 8.0, FLT_EPSILON,
                     "iteration 2 should be scaled");
  cr_expect_float_eq(linear_scale(start, end, iter_count, 9), 1.0, FLT_EPSILON,
                     "iteration 9 should be at end");
}


Test(utils_unit, random_min_max) {
  srand((unsigned) time(NULL));

  double r = random_min_max(0.0, 1.0);
  cr_expect_leq(r, 1.0, "random_min_max upper bound 1");
  cr_expect_geq(r, 0.0, "random_min_max lower bound 1");

  r = random_min_max(-5.0, 0.0);
  cr_expect_leq(r, 0.0, "random_min_max upper bound 2");
  cr_expect_geq(r, -5.0, "random_min_max lower bound 2");

  r = random_min_max(-100.0, 100.0);
  cr_expect_leq(r, 100.0, "random_min_max upper bound 3");
  cr_expect_geq(r, -100.0, "random_min_max lower bound 3");

  r = random_min_max(0.0, 0.0);
  cr_expect_leq(r, 0.0, "random_min_max upper bound 4");
  cr_expect_geq(r, 0.0, "random_min_max lower bound 4");
}


Test(utils_unit, identity) {
  size_t dim = 10;
  double matrix[dim * dim];
  identity(dim, matrix);
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      cr_expect_float_eq(matrix[row * dim + col],
                         (row == col ? 1.0 : 0.0),
                         FLT_EPSILON,
                         "entry [%ld, %ld] should be correct", row, col);
    }
  }
}


Test(utils_unit, mmm_ident) {
  size_t dim = 10;
  double res[dim * dim];
  double random[dim * dim];
  double ident[dim * dim];
  identity(dim, ident);
  for(size_t idx = 0; idx < dim * dim; idx++) {
    random[idx] = random_0_to_1();
  }
  mmm(dim, random, ident, res);
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      cr_expect_float_eq(random[row * dim + col],
                         res[row * dim + col],
                         FLT_EPSILON,
                         "entry [%ld, %ld] should be the same", row, col);
    }
  }
}


Test(utils_unit, mmm_rotation) {
  size_t dim = 2;
  double res[] = {0.0, 0.0, 0.0, 0.0};
  double a[] = {1.0, 2.0, 3.0, 4.0};
  double b[] = {0.0, -1.0, 1.0, 0.0};
  double expected[] = {2.0, -1.0, 4.0, -3.0};
  mmm(dim, a, b, res);
  for(size_t col = 0; col < dim; col++) {
    for(size_t row = 0; row < dim; row++) {
      cr_expect_float_eq(expected[row * dim + col],
                         res[row * dim + col],
                         FLT_EPSILON,
                         "entry [%ld, %ld] should be correct", row, col);
    }
  }
}


Test(utils_unit, mvm_zero) {
  size_t dim = 100;
  double* zeros = filled_double_array(dim, 0.0);
  double* res = filled_double_array(dim, 1.0);
  double* const random = (double*)malloc(dim * dim * sizeof(double));
  for(size_t idx = 0; idx < dim * dim; idx++) {
    random[idx] = random_0_to_1();
  }
  mvm(dim, random, zeros, res);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_expect_float_eq(zeros[idx],
                       res[idx],
                       FLT_EPSILON,
                       "entry [%ld] should be zero", idx);
  }
  free(res);
  free(zeros);
  free(random);
}


Test(utils_unit, vva) {
  size_t dim = 100;
  double* a = filled_double_array(dim, 1.5);
  double* b = filled_double_array(dim, 0.1);
  double* res = filled_double_array(dim, 1.0);
  double* expected = filled_double_array(dim, 1.6);
  vva(dim, a, b, res);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_expect_float_eq(expected[idx],
                       res[idx],
                       FLT_EPSILON,
                       "entry [%ld] should be 1.6", idx);
  }
  free(res);
  free(a);
  free(b);
  free(expected);
}


Test(utils_unit, negate) {
  size_t dim = 100;
  double* const array = (double*)malloc(dim * sizeof(double));
  double* const copy = (double*)malloc(dim * sizeof(double));
  for(size_t idx = 0; idx < dim; idx++) {
    array[idx] = random_0_to_1();
  }
  memcpy(copy, array, dim * sizeof(double));
  negate(dim, array);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_expect_float_eq(array[idx], -copy[idx], FLT_EPSILON,
                       "array value should be negated");
  }
}


Test(utils_unit, mean_value_in_strides_simple) {
  size_t stride = 2;
  size_t length = 10;
  double test_array[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2};
  for (size_t offset = 0; offset < 2; offset++) {
    double mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_expect_eq(mean, offset + 1,
                 "mean value at offset %ld should be %ld", offset, offset + 1);
  }
}


Test(utils_unit, mean_value_in_strides_complex) {
  size_t stride = 3;
  size_t length = 6;
  double test_array[] = {1, 2, 3, 4, 5, 6};
  double expected[] = {5.0 / 2, 7.0 / 2, 9.0 / 2};
  for (size_t offset = 0; offset < 3; offset++) {
    double mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_expect_float_eq(mean, expected[offset], FLT_EPSILON,
                       "mean value at offset %ld should be correct", offset);
  }
}

Test(utils_unit, mean_value_in_strides_complex_2) {
  size_t stride = 2;
  size_t length = 6;
  double test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  double expected[] = {(3.0 + 5.0 + 7.0) / 3, (4.0 + 6.0 + 8.0) / 3};
  for (size_t offset = 0; offset < 2; offset++) {
    double mean = mean_value_in_strides(length, &test_array[2], offset, stride);
    cr_expect_float_eq(mean, expected[offset], FLT_EPSILON,
                       "mean value at offset %ld should be correct", offset);
  }
}

Test(utils_unit, mean_value_in_strides_single) {
  size_t stride = 2;
  size_t length = 3;
  double test_array[] = {1, 2, 3};
  double expected[] = {2.0, 2.0};
  for (size_t offset = 0; offset < 2; offset++) {
    double mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_expect_float_eq(mean, expected[offset], FLT_EPSILON,
                       "mean value at offset %ld should be correct", offset);
  }
}
