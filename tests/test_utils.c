#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include <criterion/criterion.h>

Test(utils_unit, identity) {
  size_t dim = 10;
  double* const matrix = (double*)malloc(dim * dim * sizeof(double));
  identity(dim, matrix);
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      cr_assert(matrix[row * dim + col] == (row == col ? 1.0 : 0.0), "matrix should be an identity matrix");
    }
  }
}
