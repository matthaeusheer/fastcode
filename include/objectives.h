#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

/*******************************************************************************
  OBJECTIVE FUNCTIONS PROTOTYPES
******************************************************************************/

float sum_of_squares (const float * args, size_t dim);

float sum            (const float * args, size_t dim);

float sum_negative   (const float * args, size_t dim);

float rastigrin      (const float * args, size_t dim);

float rosenbrock     (const float * args, size_t dim);

float rosenbrock_neg (const float * args, size_t dim);

float sphere         (const float * args, size_t dim);

float egghol2d       (const float * args, size_t dim);

float schaf2d        (const float * args, size_t dim);

float griewank       (const float * args, size_t dim);


/*******************************************************************************
  UTILITIES
******************************************************************************/

void print_solution(size_t dim, const float * solution);

float *filled_float_array(size_t length, float val);

#ifdef __cplusplus
}
#endif // __cplusplus
