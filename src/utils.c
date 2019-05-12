#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

float horizontal_add(__m256 a) {
  __m256 t1 = _mm256_hadd_ps(a,a);
  __m256 t2 = _mm256_hadd_ps(t1,t1);
  __m128 t3 = _mm256_extractf128_ps(t2,1);
  __m128 t4 = _mm_add_ss(_mm256_castps256_ps128(t2),t3);
  return _mm_cvtss_f32(t4);
}


// Possibly more efficient solution than above
/* float horizontal_add(__m256 x) { */
/*   __m128 hi = _mm256_extractf128_ps(x, 1); */
/*   __m128 lo = _mm256_extractf128_ps(x, 0); */
/*   lo = _mm_add_ps(hi, lo); */
/*   hi = _mm_movehl_ps(hi, lo); */
/*   lo = _mm_add_ps(hi, lo); */
/*   hi = _mm_shuffle_ps(lo, lo, 1); */
/*   lo = _mm_add_ss(hi, lo); */
/*   return _mm_cvtss_f32(lo); */
/* } */

float *filled_float_array(size_t length, float val) {
  float *res = (float *) malloc(length * sizeof(float));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

void fill_float_array(float* array, size_t length, float val) {
  for(size_t idx = 0; idx < length; idx++) {
    array[idx] = val;
  }
}

int *filled_int_array(size_t length, int val) {
  int *res = (int *) malloc(length * sizeof(int));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

void fill_int_array(int* array, size_t length, int val) {
  for(size_t idx = 0; idx < length; idx++) {
    array[idx] = val;
  }
}

size_t *filled_size_t_array(size_t length, size_t val) {
  size_t *res = (size_t *) malloc(length * sizeof(size_t));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

float random_min_max(const float min, const float max) {
  float x = (float) rand() / RAND_MAX;
  return min + x * (max - min);
}

float random_0_to_1() {
  return (float) rand() / RAND_MAX;
}

void print_solution(size_t dim, const float *const solution) {
  for (size_t idx = 0; idx < dim; idx++) {
    printf("%.4f, ", solution[idx]);
  }
  printf("\n");
}

float linear_scale(float start, float end, size_t iter_max, size_t iter) {
  float scale = end - start;
  float factor = (scale / iter_max) * iter;
  return (start + factor);
}


/**
   Prints the population to standard output.
 */
void print_population(size_t colony_size, size_t dim, const float *population) {
  for (size_t idx = 0; idx < colony_size; idx++) {
    printf("member%03ld, ", idx);
    print_solution(dim, &population[idx * dim]);
  }
}


float average_value(size_t arr_length, const float * arr) {
  float sum = 0;
  for (size_t idx = 0; idx < arr_length; ++idx) {
    sum += arr[idx];
  }
  return sum / arr_length;
}


float lowest_value(size_t arr_length, const float * arr) {
  float lowest = 10e10;
  for (size_t idx = 0; idx < arr_length; ++idx) {
    if (arr[idx] < lowest)
      lowest = arr[idx];
  }
  return lowest;
}


void mmm(size_t dim, const float* const a, const float* const b, float* const res) {
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      float sum = 0.0;
      for(size_t runner = 0; runner < dim; runner++) {
        sum += a[row * dim + runner] * b[runner * dim + col];
      }
      res[row * dim + col] = sum;
    }
  }
}


void mvm(size_t dim, const float* m, const float* v, float* res) {
  for(size_t idx = 0; idx < dim; idx++) {
    float sum = 0.0;
    for(size_t runner = 0; runner < dim; runner++) {
      sum += m[idx * dim + runner] * v[runner];
    }
    res[idx] = sum;
  }
}


void vva(size_t dim, const float* a, const float* b, float* res) {
  for(size_t idx = 0; idx < dim; idx++) {
    res[idx] = a[idx] + b[idx];
  }
}


void identity(size_t dim, float* const matrix) {
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      matrix[row * dim + col] = (col == row ? 1.0 : 0.0);
    }
  }
}


void scalar_mul(size_t size, const float scalar, float* const array) {
  for(size_t idx = 0; idx < size; idx++) {
    array[idx] = scalar * array[idx];
  }
}


void negate(size_t size, float* const array) {
  scalar_mul(size, -1.0, array);
}

float mean_value_in_strides(size_t length, const float * input, size_t offset, size_t stride) {
  float sum = 0.0;
  int counter = 0;
  for (size_t idx = offset; idx < length; idx += stride) {
    sum += input[idx];
    counter += 1;
  }
  if (counter > 0)
    sum /= counter;
  return sum;
}

void print_size_t_array(size_t length, const size_t * arr) {
  for (size_t idx = 0; idx < length; ++idx) {
    printf("\t# %lu \n", arr[idx]);
  }
  printf("#-----\n");
}

void print_int_array(size_t length, const int * arr) {
  for (size_t idx = 0; idx < length; ++idx) {
    printf("\t# %d \n", arr[idx]);
  }
  printf("#-----\n");
}

void print_float_array(size_t length, const float * arr) {
  for (size_t idx = 0; idx < length; ++idx) {
    printf("\t# %f \n", arr[idx]);
  }
  printf("#-----\n");
}
