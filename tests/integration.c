/**
   Integration tests for numerical correctness of algorithms.
*/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "../objectives/objectives.h"
#include "../penguin/main.h"
#include "../HGWOSCA/main.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


/*******************************************************************************
  INTEGRATION TESTS
******************************************************************************/

Test(hgwosca_integration, sum_of_squares) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 120;
  double* mins = filled_array(dim, -100);
  double* maxs = filled_array(dim, 100);
  double* solution = hgwosca(sum_of_squares, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.00001, "each dimension should be zero");
  }
  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "objective should be minimised at 0");
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, sum) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 100;
  double* mins = filled_array(dim, -100);
  double* maxs = filled_array(dim, 100);
  double* solution = hgwosca(sum, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] + 100) < 0.00001, "each dimension should be -100");
  }
  cr_assert(fabs(sum(solution, dim) + 100 * dim) < 0.2 * dim, "objective should be "
            "minimised at -100*dim");
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, rosenbrock) {
  size_t wolf_count = 40;
  size_t dim = 3;
  size_t max_iter = 1500;
  double* mins = filled_array(dim, -100);
  double* maxs = filled_array(dim, 100);
  double* solution = hgwosca(rosenbrock, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close "
              "to 1");
  }
  free(solution);
  free(mins);
  free(maxs);
}


Test(hgwosca_integration, sphere) {
size_t wolf_count = 30;
size_t dim = 10;
size_t max_iter = 800;
double* mins = filled_array(dim, -100);
double* maxs = filled_array(dim, 100);
double* solution = hgwosca(sphere, wolf_count, dim, max_iter, mins, maxs);
for(size_t idx = 0; idx < dim; idx++) {
cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close "
"to 0");
}
cr_assert(fabs(sphere(solution, dim)) < 0.5, "objective should be minimised at 0");
free(solution);
free(mins);
free(maxs);
}


/* Test(hgwosca_integration, rastigrin) { */
/*   size_t wolf_count = 40; */
/*   size_t dim = 10; */
/*   size_t max_iter = 100; */
/*   double* mins = filled_array(dim, -100); */
/*   double* maxs = filled_array(dim, 100); */
/*   double* solution = hgwosca(rastigrin, wolf_count, dim, max_iter, mins, maxs); */
/*   print_solution(dim, solution); */
/*   for(size_t idx = 0; idx < dim; idx++) { */
/*     cr_assert(fabs(solution[idx]) < 0.001, "each dimension should be reasonably close " */
/*               "to 0"); */
/*   } */
/*   cr_assert(fabs(rastigrin(solution, dim)) < 0.1, "objective should be minimised at 0"); */
/*   free(solution); */
/*   free(mins); */
/*   free(maxs); */
/* } */


// Function is too noisy
/* Test(hgwosca_integration, egghol2d) { */
/*   size_t wolf_count = 80; */
/*   size_t dim = 2; */
/*   size_t max_iter = 9000; */
/*   const double* const mins = filled_array(dim, -512); */
/*   const double* const maxs = filled_array(dim, 512); */
/*   double* solution = hgwosca(egghol2d, wolf_count, dim, max_iter, mins, maxs); */
/*   print_solution(dim, solution); */
/*   printf("Objective=%f\n", egghol2d(solution, dim)); */
/*   cr_assert(fabs(solution[0] - 512) < 0.1, "x should be 512"); */
/*   cr_assert(fabs(solution[1] - 404.2319) < 0.1, "y should be 404.2319"); */
/*   cr_assert(fabs(egghol2d(solution, dim) + 959.6407) < 1, "objective should be " */
/*             "minimised at -959.6407"); */
/*   free(solution); */
/* } */


// Function is too noisy
/* Test(hgwosca_integration, schaf2d) { */
/*   size_t wolf_count = 40; */
/*   size_t dim = 2; */
/*   size_t max_iter = 9000; */
/*   const double* const mins = filled_array(dim, -100); */
/*   const double* const maxs = filled_array(dim, 100); */
/*   double* solution = hgwosca(schaf2d, wolf_count, dim, max_iter, mins, maxs); */
/*     print_solution(dim, solution); */
/*     printf("Objective=%f\n", schaf2d(solution, dim)); */
/*   for(size_t idx = 0; idx < dim; idx++) { */
/*     cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close " */
/*               "to 0"); */
/*   } */
/*   cr_assert(fabs(schaf2d(solution, dim)) < 0.5, "objective should be minimised at 0"); */
/*   free(solution); */
/* } */


/* Test(hgwosca_integration, griewank) { */
/*   size_t wolf_count = 30; */
/*   size_t dim = 5; */
/*   size_t max_iter = 800; */
/*   double* mins = filled_array(dim, -600); */
/*   double* maxs = filled_array(dim, 600); */
/*   double* solution = hgwosca(griewank, wolf_count, dim, max_iter, mins, maxs); */
/*   print_solution(dim, solution); */
/*   printf("Objective=%f\n", schaf2d(solution, dim)); */
/*   for(size_t idx = 0; idx < dim; idx++) { */
/*     cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close " */
/*               "to 0"); */
/*   } */
/*   cr_assert(fabs(sphere(solution, dim)) < 0.5, "objective should be minimised at 0"); */
/*   free(solution); */
/*   free(mins); */
/*   free(maxs); */
/* } */


/*******************************************************************************
  IMPLEMENTATION UNIT TESTS (used once objective functions get optimized)
******************************************************************************/

Test(obj_unit, sum) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_assert(abs(sum(args, 6) - 15.0) < DBL_EPSILON, "sum function works as expected.");
}
