#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Prints the solution array of one algorithm output to console.
 */
void print_solution(size_t dim, const double * solution);

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

#ifdef __cplusplus
}
#endif // __cplusplus