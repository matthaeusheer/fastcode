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
 * Compute matrix matrix multiplication between `a` and `b`, both square matrices of size
 * `dim` times `dim` and store the result in `res`.
 */
void mmm(size_t dim, double* a, double* b, double* res);

/**
 * Overwrites `matrix` of size `dim` times `dim` to be an identity matrix.
 */
void identity(size_t dim, double* matrix);

#ifdef __cplusplus
}
#endif // __cplusplus
