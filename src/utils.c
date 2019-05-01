
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"


double *filled_double_array(size_t length, double val) {
  double *res = (double *) malloc(length * sizeof(double));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

int *filled_int_array(size_t length, int val) {
  int *res = (int *) malloc(length * sizeof(int));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

size_t *filled_size_t_array(size_t length, size_t val) {
  size_t *res = (size_t *) malloc(length * sizeof(size_t));
  for (size_t idx = 0; idx < length; idx++) {
    res[idx] = val;
  }
  return res;
}

double random_min_max(const double min, const double max) {
  double x = (double) rand() / RAND_MAX;
  return min + x * (max - min);
}

double random_0_to_1() {
  return (double) rand() / RAND_MAX;
}

void print_solution(size_t dim, const double *const solution) {
  for (size_t idx = 0; idx < dim; idx++) {
    printf("%.4f, ", solution[idx]);
  }
  printf("\n");
}

double linear_scale(double start, double end, size_t iter_max, size_t iter) {
  double scale = end - start;
  double factor = (scale / iter_max) * iter;
  return (start + factor);
}


/**
   Prints the population to standard output.
 */
void print_population(size_t colony_size, size_t dim, const double *population) {
  for (size_t idx = 0; idx < colony_size; idx++) {
    printf("member%03ld, ", idx);
    print_solution(dim, &population[idx * dim]);
  }
}


double average_value(size_t arr_length, const double * arr) {
  double sum = 0;
  for (size_t idx = 0; idx < arr_length; ++idx) {
    sum += arr[idx];
  }
  return sum / arr_length;
}


void mmm(size_t dim, const double* const a, const double* const b, double* const res) {
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      double sum = 0.0;
      for(size_t runner = 0; runner < dim; runner++) {
        sum += a[row * dim + runner] * b[runner * dim + col];
      }
      res[row * dim + col] = sum;
    }
  }
}


void mvm(size_t dim, const double* m, const double* v, double* res) {
  for(size_t idx = 0; idx < dim; idx++) {
    double sum = 0.0;
    for(size_t runner = 0; runner < dim; runner++) {
      sum += m[idx * dim + runner] * v[runner];
    }
    res[idx] = sum;
  }
}


void vva(size_t dim, const double* a, const double* b, double* res) {
  for(size_t idx = 0; idx < dim; idx++) {
    res[idx] = a[idx] + b[idx];
  }
}


void identity(size_t dim, double* const matrix) {
  for(size_t row = 0; row < dim; row++) {
    for(size_t col = 0; col < dim; col++) {
      matrix[row * dim + col] = (col == row ? 1.0 : 0.0);
    }
  }
}


void scalar_mul(size_t size, const double scalar, double* const array) {
  for(size_t idx = 0; idx < size; idx++) {
    array[idx] = scalar * array[idx];
  }
}


void negate(size_t size, double* const array) {
  scalar_mul(size, -1.0, array);
}

double mean_value_in_strides(size_t length, const double * input, size_t offset, size_t stride) {
  double sum = 0.0;
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

void print_double_array(size_t length, const double * arr) {
  for (size_t idx = 0; idx < length; ++idx) {
    printf("\t# %f \n", arr[idx]);
  }
  printf("#-----\n");
}
