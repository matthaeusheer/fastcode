#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
double *filled_array(size_t length, double val);

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

#ifdef __cplusplus
}
#endif // __cplusplus
