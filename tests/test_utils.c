#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include <criterion/criterion.h>

Test(utils_unit, random_0_to_1) {
  for(size_t iter = 0; iter < 20; iter++) {
    double r = random_0_to_1();
    cr_assert(r <= 1, "random_0_to_1 should be bound above by 1");
    cr_assert(r >= 0, "random_0_to_1 should be bound below by 0");
  }
}


Test(utils_unit, random_min_max) {
  double r = random_min_max(0.0, 1.0);
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
  double* const matrix = (double*)malloc(dim * dim * sizeof(double));
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
  double* const res = (double*)malloc(dim * dim * sizeof(double));
  double* const random = (double*)malloc(dim * dim * sizeof(double));
  double* const ident = (double*)malloc(dim * dim * sizeof(double));
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
