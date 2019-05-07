#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include <criterion/criterion.h>

Test(utils_unit, random_0_to_1) {
  for(size_t iter = 0; iter < 20; iter++) {
    float r = random_0_to_1();
    cr_assert(r <= 1, "random_0_to_1 should be bound above by 1");
    cr_assert(r >= 0, "random_0_to_1 should be bound below by 0");
  }
}


Test(utils_unit, linear_scale) {
  float start = 1.0;
  float end = 10.0;
  size_t iter_count = 9;
  cr_assert_eq(linear_scale(start, end, iter_count, 0), 1.0, "iteration 0 should be at start");
  cr_assert_eq(linear_scale(start, end, iter_count, 2), 3.0, "iteration 2 should be scaled");
  cr_assert_eq(linear_scale(start, end, iter_count, 9), 10.0, "iteration 9 should be at end");

  // start > end
  start = 10.0;
  end = 1.0;
  iter_count = 9;
  cr_assert_eq(linear_scale(start, end, iter_count, 0), 10.0, "iteration 0 should be at start");
  cr_assert_eq(linear_scale(start, end, iter_count, 2), 8.0, "iteration 2 should be scaled");
  cr_assert_eq(linear_scale(start, end, iter_count, 9), 1.0, "iteration 9 should be at end");
}


Test(utils_unit, random_min_max) {
  srand((unsigned) time(NULL));


  float r = random_min_max(0.0, 1.0);
  cr_assert(r <= 1.0, "random_min_max upper bound 1");
  cr_assert(r >= 0.0, "random_min_max lower bound 1");

  r = random_min_max(-5.0, 0.0);
  cr_assert(r <= 0.0, "random_min_max upper bound 2");
  cr_assert(r >= -5.0, "random_min_max lower bound 2");

  r = random_min_max(-100.0, 100.0);
  cr_assert(r <= 100.0, "random_min_max upper bound 3");
  cr_assert(r >= -100.0, "random_min_max lower bound 3");

  r = random_min_max(0.0, 0.0);
  cr_assert(r <= 0.0, "random_min_max upper bound 4");
  cr_assert(r >= 0.0, "random_min_max lower bound 4");
}


Test(utils_unit, identity) {
  size_t dim = 10;
  float* const matrix = (float*)malloc(dim * dim * sizeof(float));
  identity(dim, matrix);
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      cr_assert(matrix[row * dim + col] == (row == col ? 1.0 : 0.0), "matrix should be an identity matrix");
    }
  }
  free(matrix);
}


Test(utils_unit, mmm_ident) {
  size_t dim = 10;
  float* const res = (float*)malloc(dim * dim * sizeof(float));
  float* const random = (float*)malloc(dim * dim * sizeof(float));
  float* const ident = (float*)malloc(dim * dim * sizeof(float));
  identity(dim, ident);
  for(size_t idx = 0; idx < dim * dim; idx++) {
    random[idx] = random_0_to_1();
  }
  mmm(dim, random, ident, res);
  cr_assert_arr_eq(random, res, dim * dim, "multiplying by identity should not change the matrix");
  free(res);
  free(random);
  free(ident);
}


Test(utils_unit, mmm_rotation) {
  size_t dim = 2;
  float res[] = {0.0, 0.0, 0.0, 0.0};
  float a[] = {1.0, 2.0, 3.0, 4.0};
  float b[] = {0.0, -1.0, 1.0, 0.0};
  float expected[] = {2.0, -1.0, 4.0, -3.0};
  mmm(dim, a, b, res);
  cr_assert_arr_eq(expected, res, dim * dim, "matrix should be rotated by 90 degrees");
}


Test(utils_unit, mvm_zero) {
  size_t dim = 100;
  float* zeros = filled_float_array(dim, 0.0);
  float* res = filled_float_array(dim, 1.0);
  float* const random = (float*)malloc(dim * dim * sizeof(float));
  for(size_t idx = 0; idx < dim * dim; idx++) {
    random[idx] = random_0_to_1();
  }
  mvm(dim, random, zeros, res);
  cr_assert_arr_eq(zeros, res, dim, "vector should be zeros everywhere");
  free(res);
  free(zeros);
  free(random);
}


Test(utils_unit, vva) {
  size_t dim = 100;
  float* a = filled_float_array(dim, 1.5);
  float* b = filled_float_array(dim, 0.1);
  float* res = filled_float_array(dim, 1.0);
  float* expected = filled_float_array(dim, 1.6);
  vva(dim, a, b, res);
  cr_assert_arr_eq(expected, res, dim, "vector should be 1.6 everywhere");
  free(res);
  free(a);
  free(b);
  free(expected);
}


Test(utils_unit, negate) {
  size_t dim = 100;
  float* const array = (float*)malloc(dim * sizeof(float));
  float* const copy = (float*)malloc(dim * sizeof(float));
  for(size_t idx = 0; idx < dim; idx++) {
    array[idx] = random_0_to_1();
  }
  memcpy(copy, array, dim * sizeof(float));
  negate(dim, array);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(array[idx] == -copy[idx], "array value should be negated");
  }
}


Test(utils_unit, mean_value_in_strides_simple) {
  size_t stride = 2;
  size_t length = 10;
  float test_array[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2};
  for (size_t offset = 0; offset < 2; offset++) {
    float mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_assert_eq(mean, offset + 1);
  }
}


Test(utils_unit, mean_value_in_strides_complex) {
  size_t stride = 3;
  size_t length = 6;
  float test_array[] = {1, 2, 3, 4, 5, 6};
  float expected[] = {5.0 / 2, 7.0 / 2, 9.0 / 2};
  for (size_t offset = 0; offset < 3; offset++) {
    float mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_assert_eq(mean, expected[offset]);
  }
}

Test(utils_unit, mean_value_in_strides_complex_2) {
  size_t stride = 2;
  size_t length = 6;
  float test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // basically take the inner 6 most items with stride equals 2
  float expected[] = {(3.0 + 5.0 + 7.0) / 3, (4.0 + 6.0 + 8.0) / 3};
  for (size_t offset = 0; offset < 2; offset++) {
    float mean = mean_value_in_strides(length, &test_array[2], offset, stride);
    cr_assert_eq(mean, expected[offset]);
  }
}

Test(utils_unit, mean_value_in_strides_single) {
  size_t stride = 2;
  size_t length = 3;
  float test_array[] = {1, 2, 3};
  // basically take the inner 6 most items with stride equals 2
  float expected[] = {2.0, 2.0};
  for (size_t offset = 0; offset < 2; offset++) {
    float mean = mean_value_in_strides(length, test_array, offset, stride);
    cr_assert_eq(mean, expected[offset]);
  }
}
