#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <immintrin.h>

#include "objectives.h"
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/*******************************************************************************
  IMPLEMENTATIONS OF OBJECTIVE FUNCTIONS TO TEST ALGORITHMS
******************************************************************************/

/**
 * Sum of squares SIMD function
 * optimal solution is 0s everywhere
 */
float simd_sum_of_squares(const float *const args, size_t dim) {
  __m256 v_sum = _mm256_setzero_ps();
  size_t idx = 0;

  if(dim > 7) {
    __m256 v_args;
    for(; idx < dim - 8; idx += 8) {
      v_args = _mm256_loadu_ps(&args[idx]);
      v_sum = _mm256_fmadd_ps(v_args, v_args, v_sum);
    }
  }

  float sum = horizontal_add(v_sum);
  for(; idx < dim; idx++) {
    sum += args[idx] * args[idx];
  }

  return sum;
}

/**
 * Sum of squares function
 * optimal solution is 0s everywhere
 */
float sum_of_squares(const float *const args, size_t dim) {
  float sum = 0;
  size_t idx = 0;
  for(; idx < dim; idx++) {
    sum += args[idx] * args[idx];
  }
  return sum;
}

/**
 * Simple summation function
 * optimal solution is 0s everywhere
 */
float sum(const float *const args, size_t dim) {
  float sum = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    sum += args[idx];
  }
  return sum;
}

float sum_negative(const float *const args, size_t dim) {
  return -1.0 * sum(args, dim);
}

/**
 * Rastigrin function
 * optimal solution is 0s everywhere
 */
float rastigrin(const float *const args, size_t dim) {
  float rstgnND = 10.0 * dim;
  for (size_t idx = 0; idx < dim; idx++) {
    rstgnND += pow(args[idx], 2) - 10.0 * cos(2 * M_PI * args[idx]);
  }
  return rstgnND;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
float rosenbrock(const float *const args, size_t dim) {
  float rNd = 0.0;
  for (size_t idx = 0; idx < dim - 1; idx++)
  rNd += (100.0 * (+pow(args[idx + 1] - pow(args[idx], 2), 2)) + pow(1 - args[idx], 2));
  return rNd;
}

/**
 * Multidimensional Rosenbrock Function
 * global minima f(x1,.....,xN) = 0 at (x1,.....,xN) = (1,.....,1)
 */
float simd_rosenbrock(const float *const args, size_t dim) {
  float rNd = 0.0;
  __m256 ones = _mm256_set1_ps(1.0);
  __m256 cent = _mm256_set1_ps(100.0);
  __m256 res = _mm256_setzero_ps();

  int idx = 0;
  if (dim > 8) {
    for (idx; idx < dim - 9; idx+=8){
      // printf("this is inside the loop\n" );
      __m256 pos = _mm256_loadu_ps(&args[idx]);
      __m256 pos_p1 = _mm256_loadu_ps(&args[idx+1]);
      __m256 r1 = _mm256_fmsub_ps(pos,pos,pos_p1);
      r1 = _mm256_mul_ps(r1,r1);
      r1 = _mm256_mul_ps(cent,r1);
      __m256 temp = _mm256_sub_ps(ones,pos);
      res = _mm256_add_ps(res, _mm256_fmadd_ps(temp,temp,r1));
    }
    rNd = horizontal_add(res);
  }
  for (; idx < dim - 1; idx++){
    rNd += (100.0 * (+pow(args[idx + 1] - pow(args[idx], 2), 2)) + pow(1 - args[idx], 2));
  }
  return rNd;
}


/**
 * Multi Dimensional Sphere Function
 * global minima at f(x1,.....,xN) = 0 at (x1,......,xN) = (0,......,0)
 */
float sphere(const float *const args, size_t dim) {
  float sph = 0.0;
  for (size_t idx = 0; idx < dim; idx++)
    sph += pow(args[idx], 2);
  return sph;
}

/**
 * 2D Eggholder Function
 * global minima at f(x,y) = -959.6407 at (x,y) = (512,404.2319)
 */
float egghol2d(const float *const args, size_t dim) {
  //assert(dim == 2);
  //assert( fabs(args[0])<=512 && fabs(args[1])<=512 );
  float eggh = -1.0 * (args[1] + 47.0) * sin(sqrt(fabs(args[0] + (args[1] + 47.0))))
                - args[0] * sin(sqrt(fabs(args[0] - (args[1] + 47.0))));
  return eggh;
}

/**
 * 2D Schaffer Function
 */
float schaf2d(const float *const args, size_t dim) {
  //float sch = 0.5 + (pow(cos(pow(sin(fabs(args[0] * args[0] - args[1] * args[1])), 2)), 2) - 0.5) /
                    // pow((1 + 0.001 * ((args[0] * args[0]) + (args[1] * args[1]))), 2);
  float sch = 0.5 + (pow(sin(args[0] * args[0] - args[1] * args[1]), 2) - 0.5) /
                    pow((1 + 0.001 * ((args[0] * args[0]) + (args[1] * args[1]))), 2);
  return sch;
}

/**
 * Griewank function
 */
float griewank(const float *const args, size_t dim) {
  float sum = 0.0, prod = 1.0;
  for (size_t idx = 1; idx < dim; idx++) {
    sum += pow(args[idx], 2) / 4000.0;
    prod *= cos(args[idx] / sqrt(idx));
  }
  return sum - prod + 1.0;
}

/**This class defines the Schwefel 1 global optimization problem.
 *This is a unimodal minimization problem defined as follows:
 */
float schwefel01(const float *const args, size_t dim){
  float sch00 = 0.0;
  float alpha = sqrt(M_PI);
  for (size_t idx = 0; idx < dim; idx++) {
    sch00 += pow(args[idx],2);
  }
  return pow(sch00, alpha);
}


/**
   This class defines the Powell global optimization problem. 	N=4,	32.58%
   This is a multimodal minimization problem defined as follows */

float powel(const float *const args, size_t dim){
  float powel = pow((args[0] + 10.0 * args[1]), 2) + 5.0 * pow((args[2] - args[3]), 2) + pow((args[1] - 2 * args[2]), 4) + 10.0 * pow((args[0] - args[3]), 4);
  return powel;
}


/**
   41.58%, N=2
   This class defines the Freudenstein & Roth global
   optimization problem. This is a multimodal minimization problem defined as follows:
*/

float freundsteinroth(const float *const args, size_t dim) {
  float fr = pow((args[0] - 13.0 + ((5.0 - args[1]) * args[1] - 2) * args[1]), 2) + pow((args[0] - 29.0 + ((args[1] + 1.0) * args[1] - 14.0) * args[1]), 2);
  return fr;
}

/**
 *Beale
 This class defines the Beale global optimization problem. This is a multimodal minimization problem defined as follows:
*/
float beale(const float *const args, size_t dim) {
  float beal = pow((args[0] * args[1] - args[0] + 1.5), 2) + pow((args[0] * args[1] * args[1] - args[0] + 2.25), 2) + pow((args[0] * args[1] * args[1] * args[1] - args[0] + 2.625), 2);
  return beal;
}
