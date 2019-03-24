/**
   Integration tests for numerical correctness of algorithms.
*/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "../penguin/main.h"
#include "../HGWOSCA/main.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/*******************************************************************************
  SAMPLE OBJECTIVE FUNCTIONS
******************************************************************************/

double sum_of_squares(const double* const args, size_t dim);
double sum(const double* const args, size_t dim);
double rastigrin(const double* const args, size_t dim);
double rosenbrock(const double* const args, size_t dim);
double sphere(const double* const args, size_t dim);
double egghol2d(const double* const args, size_t dim);
double schaf2d(const double* const args, size_t dim);

/*******************************************************************************
  UTILITIES
******************************************************************************/

void print_solution(size_t dim, const double* const solution);
double* filled_array(size_t length, double val);


/*******************************************************************************
  INTEGRATION TESTS
******************************************************************************/

Test(hgwosca_integration, sum_of_squares) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 500;
  const double* const mins = filled_array(dim, -100);
  const double* const maxs = filled_array(dim, 100);
  double* solution = hgwosca(sum_of_squares, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.01, "each dimension should be reasonably close to zero");
  }
  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "objective should be minimised at 0");
  free(solution);
}


Test(hgwosca_integration, sum) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 800;
  const double* const mins = filled_array(dim, -100);
  const double* const maxs = filled_array(dim, 100);
  double* solution = hgwosca(sum, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] + 100) < 0.5, "each dimension should be reasonably close "
              "to -100");
  }
  cr_assert(fabs(sum(solution, dim) + 100 * dim) < 0.2 * dim, "objective should be "
            "minimised at -100*dim");
  free(solution);
}


Test(hgwosca_integration, rastigrin) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 800;
  const double* const mins = filled_array(dim, -100);
  const double* const maxs = filled_array(dim, 100);
  double* solution = hgwosca(rastigrin, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close "
              "to 0");
  }
  cr_assert(fabs(rastigrin(solution, dim)) < 0.1, "objective should be minimised at 0");
  free(solution);
}


// The following takes very long, hence commented out.
/* Test(hgwosca_integration, rosenbrock) { */
/*   size_t wolf_count = 500; */
/*   size_t dim = 5; */
/*   size_t max_iter = 200000; */
/*   const double* const mins = filled_array(dim, -100); */
/*   const double* const maxs = filled_array(dim, 100); */
/*   double* solution = hgwosca(rosenbrock, wolf_count, dim, max_iter, mins, maxs); */
/*   print_solution(dim, solution); */
/*   for(size_t idx = 0; idx < dim; idx++) { */
/*     cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close " */
/*               "to 1"); */
/*   } */
/*   free(solution); */
/* } */


Test(hgwosca_integration, sphere) {
  size_t wolf_count = 30;
  size_t dim = 10;
  size_t max_iter = 800;
  const double* const mins = filled_array(dim, -100);
  const double* const maxs = filled_array(dim, 100);
  double* solution = hgwosca(sphere, wolf_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx]) < 0.1, "each dimension should be reasonably close "
              "to 0");
  }
  cr_assert(fabs(sphere(solution, dim)) < 0.5, "objective should be minimised at 0");
  free(solution);
}


// Function is too noisy
/* Test(hgwosca_integration, egghol2d) { */
/*   size_t wolf_count = 400; */
/*   size_t dim = 2; */
/*   size_t max_iter = 4000; */
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
/*   size_t wolf_count = 10; */
/*   size_t dim = 2; */
/*   size_t max_iter = 5000; */
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

/*******************************************************************************
  IMPLEMENTATIONS
******************************************************************************/

void print_solution(size_t dim, const double* const solution) {
  for(size_t idx = 0; idx < dim; idx++) {
    printf("Item %ld:\t%f\n", idx, solution[idx]);
  }
}

double* filled_array(size_t length, double val) {
  double * res = (double*)malloc(length * sizeof(double));
  for(size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

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

/** Multidimensional Rosenbrock Function
 global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)*/
double rosenbrock(const double* const args, size_t dim){
  double rNd =0.;
  for (size_t idx=0; idx<dim-1;idx++)
      rNd += ( 100*(pow( args[idx+1] - pow(args[idx],2),2)) + pow(1-args[idx],2) );
return rNd;
}

/* Multi Dimensional Sphere Function
 global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)*/
double sphere(const double* const args, size_t dim){
  double sph = 0;
  for (size_t idx=0; idx<dim ; idx++)
      sph +=  pow(args[idx],2);
return sph;
}

/**2D Eggholder Function
	global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)*/
double egghol2d(const double* const args, size_t dim){
  //assert(dim == 2);
  //assert( fabs(args[0])<=512 && fabs(args[1])<=512 );
  double eggh = -1*(args[1]+47)*sin(sqrt(fabs(args[0]+(args[1]+47))))
                  		- args[0]*sin(sqrt(fabs(args[0]-(args[1]+47))));
return eggh;
}

/** 2D Schaffer Function */
double schaf2d(const double* const args, size_t dim){
  double sch = 0.5 + (pow(cos(pow(sin(fabs(args[0]*args[0]-args[1]*args[1])),2)),2) -0.5)/
  										pow( (1+0.001*((args[0]*args[0]) + (args[1]*args[1]))), 2 );
return sch;
}


/*******************************************************************************
  IMPLEMENTATION UNIT TESTS (used once objective functions get optimized)
******************************************************************************/

Test(obj_unit, sum) {
  double args[] = {0, 1, 2, 3, 4, 5};
  cr_assert(abs(sum(args, 6) - 15.0) < DBL_EPSILON, "sum function works as expected.");
}
