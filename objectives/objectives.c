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
double sum_of_squares(const double* const args, size_t dim) {
    double sum = 0.0;
    for(size_t idx = 0; idx < dim; idx++) {
        sum += args[idx] * args[idx];
    }
    return sum;
}

/**
 * Simple summation function
 * optimal solution is 0s everywhere
 */
double sum(const double* const args, size_t dim) {
    double sum = 0.0;
    for(size_t idx = 0; idx < dim; idx++) {
        sum += args[idx];
    }
    return sum;
}

/**
 * Rastigrin function
 * optimal solution is 0s everywhere
 */
double rastigrin(const double* const args, size_t dim) {
    double rstgnND = 10 * dim;
    for(size_t idx = 0; idx < dim; idx++) {
        rstgnND += pow(args[dim], 2.0) - 10 * cos(2.0 * M_PI * args[dim]);
    }
    return rstgnND;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
double rosenbrock(const double* const args, size_t dim){
    double rNd =0.;
    for (size_t idx=0; idx<dim-1;idx++)
        rNd += ( 100*(pow( args[idx+1] - pow(args[idx],2),2)) + pow(1-args[idx],2) );
    return rNd;
}

/**
 * Multi Dimensional Sphere Function
 * global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
 */
double sphere(const double* const args, size_t dim){
    double sph = 0;
    for (size_t idx=0; idx<dim ; idx++)
        sph +=  pow(args[idx],2);
    return sph;
}

/**
 * 2D Eggholder Function
 * global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
 */
double egghol2d(const double* const args, size_t dim){
    //assert(dim == 2);
    //assert( fabs(args[0])<=512 && fabs(args[1])<=512 );
    double eggh = -1*(args[1]+47)*sin(sqrt(fabs(args[0]+(args[1]+47))))
                  - args[0]*sin(sqrt(fabs(args[0]-(args[1]+47))));
    return eggh;
}

/**
 * 2D Schaffer Function
 */
double schaf2d(const double* const args, size_t dim){
    double sch = 0.5 + (pow(cos(pow(sin(fabs(args[0]*args[0]-args[1]*args[1])),2)),2) -0.5)/
                       pow( (1+0.001*((args[0]*args[0]) + (args[1]*args[1]))), 2 );
    return sch;
}


/**
 * Griewank function
 */
double griewank(const double* const args, size_t dim) {
    double sum = 0, prod = 1;
    for(size_t idx = 0; idx < dim; idx++) {
        sum += pow(args[idx], 2) / 4000;
        prod *= cos(args[idx] / sqrt(idx));
    }
    return sum - prod + 1;
}


/*******************************************************************************
  UTILITY FUNCTIONS
******************************************************************************/

void print_solution(size_t dim, const double* const solution) {
    for(size_t idx = 0; idx < dim; idx++) {
        printf("Item %ld:\t%f\n", idx, solution[idx]);
    }
}

/**
 * Fills an double array with a given length and unique value for all entries.
 */
double* filled_array(size_t length, double val) {
    double * res = (double*)malloc(length * sizeof(double));
    for(size_t idx = 0; idx < length; idx++) {
        res[idx] = val;
    }
    return res;
}