#include <math.h>
#include <stdio.h>

#include "objectives.h"

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
float sum_of_squares(const float *const args, size_t dim) {
  float sum = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += args[idx] * args[idx];
  }
  return sum;
}

/**
 * Simple summation function
 * optimal solution is 0s everywhere
 */
float sum(const float *const args, size_t dim) {
  float sum = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += args[idx];
  }
  return sum;
}

float sum_negative(const float *const args, size_t dim) {
  return -1.0 * sum(args, dim);
}

/**
 * Rastigrin function
 * optimal solution is 0s everywhere
 */
float rastigrin(const float *const args, size_t dim) {
  float rstgnND = 10 * dim;
  for (size_t idx = 0; idx < dim; idx++) {
    rstgnND += pow(args[dim], 2.0) - 10 * cos(2.0 * M_PI * args[dim]);
  }
  return rstgnND;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
float rosenbrock(const float *const args, size_t dim) {
  float rNd = 0.;
  for (size_t idx = 0; idx < dim - 1; idx++)
    rNd += (100 * (pow(args[idx + 1] - pow(args[idx], 2), 2)) + pow(1 - args[idx], 2));
  return rNd;
}

float rosenbrock_neg(const float *const args, size_t dim) {
  return -1.0 * rosenbrock(args, dim);
}

/**
 * Multi Dimensional Sphere Function
 * global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
 */
float sphere(const float *const args, size_t dim) {
  float sph = 0;
  for (size_t idx = 0; idx < dim; idx++)
    sph += pow(args[idx], 2);
  return sph;
}

/**
 * 2D Eggholder Function
 * global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
 */
float egghol2d(const float *const args, size_t dim) {
  //assert(dim == 2);
  //assert( fabs(args[0])<=512 && fabs(args[1])<=512 );
  float eggh = -1 * (args[1] + 47) * sin(sqrt(fabs(args[0] + (args[1] + 47))))
                - args[0] * sin(sqrt(fabs(args[0] - (args[1] + 47))));
  return eggh;
}

/**
 * 2D Schaffer Function
 */
float schaf2d(const float *const args, size_t dim) {
  float sch = 0.5 + (pow(cos(pow(sin(fabs(args[0] * args[0] - args[1] * args[1])), 2)), 2) - 0.5) /
                     pow((1 + 0.001 * ((args[0] * args[0]) + (args[1] * args[1]))), 2);
  return sch;
}

/**
 * Griewank function
 */
float griewank(const float *const args, size_t dim) {
  float sum = 0, prod = 1;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += pow(args[idx], 2) / 4000;
    prod *= cos(args[idx] / sqrt(idx));
  }
  return sum - prod + 1;
}
