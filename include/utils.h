#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <immintrin.h>


// Objective function type
typedef float (*obj_func_t)(const float *, size_t);

// Algorithm function type
typedef float * (*algo_func_t)(obj_func_t, size_t, size_t, size_t, const float, const float);

float horizontal_add(__m256 a);

/**
 *  Prints the solution array of one algorithm output to console.
 */
void print_solution(size_t dim, const float * solution);

/**
   Prints the population to standard output.
 */
void print_population(size_t colony_size, size_t dim, const float *population);

/**
 * Fills an float array with a given length and unique value for all entries.
 */
float *filled_float_array(size_t length, float val);

void fill_float_array(float* array, size_t length, float val);

size_t *filled_size_t_array(size_t length, size_t val);

int *filled_int_array(size_t length, int val);

void fill_int_array(int* array, size_t length, int val);

/**
   Generate a random float between min and max.
 */
float random_min_max(float min, float max);

/**
   Generate a random float 0 and 1.
 */
float random_0_to_1();

/**
    Linearly scale a value between `start` and `end` by a ratio of `iter_max`/`iter`.
 */
float linear_scale(float start, float end, size_t iter_max, size_t iter);

/**
 * Get average value of an array.
 */
float average_value(size_t arr_length, const float * arr);

/**
 * Get lowest value of an array.
 */
float lowest_value(size_t arr_length, const float * arr);

/**
   Compute matrix matrix multiplication between `a` and `b`, both square matrices of size
   `dim` times `dim` and store the result in `res`.
 */
void mmm(size_t dim, const float* a, const float* b, float* res);

/**
   Compute matrix vector multiplication between `m` (matrix) and `v` (vector), where `m` is
   `dim` times `dim` and store the result in `res`.
 */
void mvm(size_t dim, const float* m, const float* v, float* res);

/**
   Compute vector vector addition between `a` and `b`, both having a length of size `dim`
   and store the result in `res`.
 */
void vva(size_t dim, const float* a, const float* b, float* res);

/**
   Overwrites `matrix` of size `dim` times `dim` to be an identity matrix.
 */
void identity(size_t dim, float* matrix);

/**
   Negates all values within `array` of size `size`.
 */
void negate(size_t size, float* array);

/**
   Scalar multiplication of an array.
 */
void scalar_mul(size_t size, float scalar, float* array);
 /*
 * Calc mean values in strides and offset. Assumes length of input is divisible by stride.
 */
float mean_value_in_strides(size_t length, const float * input, size_t offset, size_t stride);

void print_size_t_array(size_t length, const size_t * arr);
void print_float_array(size_t length, const float * arr);
void print_int_array(size_t length, const int * arr);


#ifdef __cplusplus
}
#endif // __cplusplus
