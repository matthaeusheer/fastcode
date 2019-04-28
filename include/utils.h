#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


// Objective function type
typedef double (*obj_func_t)(const double *, size_t);

/**
 *  Prints the solution array of one algorithm output to console.
 */
void print_solution(size_t dim, const double * solution);

/**
   Prints the population to standard output.
 */
void print_population(size_t colony_size, size_t dim, const double *population);

/**
 * Fills an double array with a given length and unique value for all entries.
 */
double *filled_double_array(size_t length, double val);

size_t *filled_size_t_array(size_t length, size_t val);

int *filled_int_array(size_t length, int val);

/**
   Generate a random double between min and max.
 */
double random_min_max(double min, double max);

/**
   Generate a random double 0 and 1.
 */
double random_0_to_1();

/**
 * Get average value of an array.
 */
double average_value(size_t arr_length, const double * arr);

/**
   Compute matrix matrix multiplication between `a` and `b`, both square matrices of size
   `dim` times `dim` and store the result in `res`.
 */
void mmm(size_t dim, const double* a, const double* b, double* res);

/**
   Compute matrix vector multiplication between `m` (matrix) and `v` (vector), where `m` is
   `dim` times `dim` and store the result in `res`.
 */
void mvm(size_t dim, const double* m, const double* v, double* res);

/**
   Compute vector vector addition between `a` and `b`, both having a length of size `dim`
   and store the result in `res`.
 */
void vva(size_t dim, const double* a, const double* b, double* res);

/**
   Overwrites `matrix` of size `dim` times `dim` to be an identity matrix.
 */
void identity(size_t dim, double* matrix);

/**
   Negates all values within `array` of size `size`.
 */
void negate(size_t size, double* array);

/**
   Scalar multiplication of an array.
 */
void scalar_mul(size_t size, double scalar, double* array);
 /*
 * Calc mean values in strides and offset. Assumes length of input is divisible by stride.
 */
double mean_value_in_strides(size_t length, const double * input, size_t offset, size_t stride);

void print_size_t_array(size_t length, const size_t * arr);
void print_double_array(size_t length, const double * arr);
void print_int_array(size_t length, const int * arr);

#ifdef __cplusplus
}
#endif // __cplusplus
