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
double sum_of_squares(const double *const args, size_t dim) {
  double sum = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += args[idx] * args[idx];
  }
  return sum;
}

/**
* Sum of cosines
* will always converge
*/ 
double cosinemixture(const double *cost args, size_t dim){
  double cosmix = 0.0;
  double cosmix1 = 0.0;
  for(size_t idx = 0; idx < dim; idx++) {
     cosmix += - args[idx] * args[idx];
     cosmix1 += - 0.1*cos(5*M_PI*args[idx]); 
  }
  return cosmix + cosmix1;
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
  double rstgnND = 10 * dim;
  for (size_t idx = 0; idx < dim; idx++) {
    rstgnND += pow(args[dim], 2.0) - 10 * cos(2.0 * M_PI * args[dim]);
  }
  return rstgnND;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
double rosenbrock(const double *const args, size_t dim) {
  double rNd = 0.;
  for (size_t idx = 0; idx < dim - 1; idx++)
    rNd += (100 * (pow(args[idx + 1] - pow(args[idx], 2), 2)) + pow(1 - args[idx], 2));
  return rNd;
}

double rosenbrock_neg(const double *const args, size_t dim) {
  return -1.0 * rosenbrock(args, dim);
}


/**
41.58%, N=2
This class defines the Freudenstein & Roth global 
optimization problem. This is a multimodal minimization problem defined as follows:
*/
double freundsteinroth(const double *const args, size_t dim) {
  double fr = pow((args[0] - 13 + ((5 - args[1]) * args[1] - 2) * args[1]), 2)
              + pow((args[0] - 29 + ((args[1] + 1) * args[1] - 14) * args[1]), 2);
  return fr;
}


/**
This class defines the Powell global optimization problem. 	N=4,	32.58%
This is a multimodal minimization problem defined as follows
  */
double Powel(const double *const args, size_t dim){
  double powel = pow((arg[2] + 10 * arg[0]), 2) + 5 * pow((arg[1] - arg[3]), 2) + pow((arg[0] - 2 * arg[1]), 4) + 10 * pow((arg[2] - arg[3]), 4) ;
  return powel;
}

/**
 * Multi Dimensional Sphere Function
 * global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
 */
double sphere(const double *const args, size_t dim) {
  double sph = 0;
  for (size_t idx = 0; idx < dim; idx++)
    sph += pow(args[idx], 2);
  return sph;
}

/**This class defines the Schwefel 1 global optimization problem. 
This is a unimodal minimization problem defined as follows:

*/
double schwefel01(const double *const args, size_t dim){
  double sch00 = 0;
  double alpha = sqrt(M_PI)
  for (size_t idx = 0; idx < dim; idx++) {
    sch00 += pow(args[idx],2);
  }
  return pow(sch00, alpha);
}  


/**
 * 2D Eggholder Function
 * global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
 */
double egghol2d(const double *const args, size_t dim) {
  //assert(dim == 2);
  //assert( fabs(args[0])<=512 && fabs(args[1])<=512 );
  double eggh = -1 * (args[1] + 47) * sin(sqrt(fabs(args[0] + (args[1] + 47))))
                - args[0] * sin(sqrt(fabs(args[0] - (args[1] + 47))));
  return eggh;
}

/**
 * 2D Schaffer Function
 */
double schaf2d(const double *const args, size_t dim) {
  double sch = 0.5 + (pow(cos(pow(sin(fabs(args[0] * args[0] - args[1] * args[1])), 2)), 2) - 0.5) /
                     pow((1 + 0.001 * ((args[0] * args[0]) + (args[1] * args[1]))), 2);
  return sch;
}

/**
 * Griewank function
 */
double griewank(const double *const args, size_t dim) {
  double sum = 0, prod = 1;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += pow(args[idx], 2) / 4000;
    prod *= cos(args[idx] / sqrt(idx));
  }
  return sum - prod + 1;
}

/**
*Beale 
This class defines the Beale global optimization problem. This is a multimodal minimization problem defined as follows:
*/
double beale(const double *const args, size_t dim) {
double beal = pow((args[0] * args[2] - args[0] + 1.5), 2) + pow((args[0] * pow(arg[1], 2)), 2) - args[0] + 2.25), 2)
              + pow((pow(args[0] * args[1], 3) - args[0] + 2.625), 2);
return beal;  
}




