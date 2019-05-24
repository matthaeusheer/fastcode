#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "objectives.h"
#include "utils.h"
#include <assert.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/*******************************************************************************
  IMPLEMENTATIONS OF OBJECTIVE FUNCTIONS TO TEST ALGORITHMS
******************************************************************************/

/**
 * Sum of squares function
 * optimal solution is 0s everywhere
 */
double sum_of_squares(const double *const args, size_t dim) {
  size_t idx = 0;
  double sum = 0;
  for(; idx < dim; idx++) {
    sum += args[idx] * args[idx];
  }
  return sum;
}

/**
 * Simple summation function
 * optimal solution is 0s everywhere
 */
double sum(const double *const args, size_t dim) {
  double sum = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += args[idx];
  }
  return sum;
}

double sum_negative(const double *const args, size_t dim) {
  return -1.0 * sum(args, dim);
}

/**
 * Rastigrin function
 * optimal solution is 0s everywhere
 */
double rastigrin(const double *const args, size_t dim) {
  double rstgnND = 10.0 * dim;
  for (size_t idx = 0; idx < dim; idx++) {
    rstgnND += pow(args[idx], 2) - 10.0 * cos(2 * M_PI * args[idx]);
  }
  return rstgnND;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
double rosenbrock(const double *const args, size_t dim) {
  double rNd = 0.0;
  for (size_t idx = 0; idx < dim - 1; idx++)
    rNd += (100.0 * (pow(args[idx + 1] - pow(args[idx], 2), 2)) + pow(1 - args[idx], 2));
  return rNd;
}


/**
 * Multi Dimensional Sphere Function
 * global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
 */
double sphere(const double *const args, size_t dim) {
  double sph = 0.0;
  for (size_t idx = 0; idx < dim; idx++)
    sph += pow(args[idx], 2);
  return sph;
}

/**
 * 2D Eggholder Function
 * global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
 */
double egghol2d(const double *const args, size_t dim) {
  assert(dim == 2);
  double eggh = -1.0 * (args[1] + 47.0) * sin(sqrt(fabs(args[0] + (args[1] + 47.0))))
                - args[0] * sin(sqrt(fabs(args[0] - (args[1] + 47.0))));
  return eggh;
}

/**
 * 2D Schaffer Function
 */
double schaf2d(const double *const args, size_t dim) {
  assert(dim == 2);
  double sch = 0.5 + (pow(sin(args[0] * args[0] - args[1] * args[1]), 2) - 0.5) /
                    pow((1 + 0.001 * ((args[0] * args[0]) + (args[1] * args[1]))), 2);
  return sch;
}

/**
 * Griewank function
 */
double griewank(const double *const args, size_t dim) {
  double sum = 0.0, prod = 1.0;
  for (size_t idx = 1; idx < dim; idx++) {
    sum += pow(args[idx], 2) / 4000.0;
    prod *= cos(args[idx] / sqrt(idx));
  }
  return sum - prod + 1.0;
}

/**This class defines the Schwefel 1 global optimization problem.
 *This is a unimodal minimization problem defined as follows:
 */
double schwefel01(const double *const args, size_t dim){
  assert(dim == 4);
  double sch00 = 0.0;
  double alpha = sqrt(M_PI);
  for (size_t idx = 0; idx < dim; idx++) {
    sch00 += pow(args[idx],2);
  }
  return pow(sch00, alpha);
}


/**
   This class defines the Powell global optimization problem. 	N=4,	32.58%
   This is a multimodal minimization problem defined as follows */

double powel(const double *const args, size_t dim){
  assert(dim == 4);
  double powel = pow((args[0] + 10.0 * args[1]), 2) + 5.0 * pow((args[2] - args[3]), 2) + pow((args[1] - 2 * args[2]), 4) + 10.0 * pow((args[0] - args[3]), 4);
  return powel;
}


/**
   41.58%, N=2
   This class defines the Freudenstein & Roth global
   optimization problem. This is a multimodal minimization problem defined as follows:
*/

double freundsteinroth(const double *const args, size_t dim) {
  assert(dim == 2);
  double fr = pow((args[0] - 13.0 + ((5.0 - args[1]) * args[1] - 2) * args[1]), 2) + pow((args[0] - 29.0 + ((args[1] + 1.0) * args[1] - 14.0) * args[1]), 2);
  return fr;
}

/**
 *Beale
 This class defines the Beale global optimization problem. This is a multimodal minimization problem defined as follows:
*/
double beale(const double *const args, size_t dim) {
  assert(dim == 2);
  double beal = pow((args[0] * args[1] - args[0] + 1.5), 2) + pow((args[0] * args[1] * args[1] - args[0] + 2.25), 2) + pow((args[0] * args[1] * args[1] * args[1] - args[0] + 2.625), 2);
  return beal;
}
