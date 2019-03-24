/**
   Integration tests for numerical correctness of algorithms.
*/

#include <math.h>
#include <float.h>

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
