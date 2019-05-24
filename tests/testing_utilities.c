#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <criterion/criterion.h>

#include "utils.h"
#include "testing_utilities.h"

void test_algo(obj_func_t obj_func, size_t pop_size, size_t dim,
               float min_bound, float max_bound,
               size_t max_iter, algo_func_t algo,
               float target, float tolerance,
               bool debug, char* suite, char* test) {
  float* solution = (*algo)(obj_func, pop_size, dim, max_iter, min_bound, max_bound);
  if(debug) {
    printf("%s -- %s\n  Best solution: ", suite, test);
    print_solution(dim, solution);
    printf("  Objective function value = %f\n", (*obj_func)(solution, dim));
    puts("--");
  }
  cr_expect_float_eq((*obj_func)(solution, dim), target, tolerance,
                     "objective should be minimised at %f", target);
  free(solution);
}

void test_simd_algo(simd_obj_func_t obj_func, size_t pop_size, size_t dim,
                    float min_bound, float max_bound,
                    size_t max_iter, simd_algo_func_t algo,
                    float target, float tolerance,
                    bool debug, char* suite, char* test) {
  float* solution = (*algo)(obj_func, pop_size, dim, max_iter, min_bound, max_bound);
  __m256 tmp[dim / 8];
  for (size_t idx = 0; idx < dim / 8; idx++) {
    tmp[idx] = _mm256_loadu_ps(&solution[idx * 8]);
  }
  if(debug) {
    printf("%s -- %s\n  Best solution: ", suite, test);
    print_solution(dim, solution);
    printf("  Objective function value = %f\n", (*obj_func)(tmp, dim / 8));
    puts("--");
  }
  cr_expect_float_eq((*obj_func)(tmp, dim / 8), target, tolerance,
                     "objective should be minimised at %f", target);
  free(solution);
}
