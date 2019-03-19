/**
   Integration tests for numerical correctness of algorithms.
*/

#include <math.h>
#include <float.h>

#include <criterion/criterion.h>

#include "../penguin/main.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/*******************************************************************************
  SAMPLE OBJECTIVE FUNCTIONS
******************************************************************************/

double sum_of_squares(const double* const args, size_t dim);
double sum(const double* const args, size_t dim);
double rastigrin(const double* const args, size_t dim);


/*******************************************************************************
  INTEGRATION TESTS
******************************************************************************/














/*******************************************************************************
  IMPLEMENTATIONS
******************************************************************************/

// Optimal solution is 0s everywhere.
double sum_of_squares(const double* const args, size_t dim) {
  double sum= 0.0;
  for(size_t idx = 0; idx < dim; idx++) {
    sum += pow(args[idx], 2.0);
  }
  return sum;
}

// Optimal solution is -100s everywhere.
double sum(const double* const args, size_t dim) {
  double sum = 0.0;
  for(size_t idx = 0; idx < dim; idx++) {
    sum += args[idx];
  }
  return sum;
}

// Optimal solution is 0s everywhere.
double rastigrin(const double* const args, size_t dim) {
  double rstgnND = 10 * dim;
  for(size_t idx = 0; idx < dim; idx++) {
    rstgnND += pow(args[dim], 2.0) - 10 * cos(2.0 * M_PI * args[dim]);
  }
  return rstgnND;
}


/*******************************************************************************
  IMPLEMENTATION UNIT TESTS (used once objective functions get optimized)
******************************************************************************/

Test(obj_unit, sum) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_assert(abs(sum(args, 6) - 15.0) < DBL_EPSILON, "sum function works as expected.");
}
