#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

/*******************************************************************************
  OBJECTIVE FUNCTIONS PROTOTYPES
******************************************************************************/

double sum_of_squares (const double * args, size_t dim);

double sum            (const double * args, size_t dim);

double sum_negative   (const double * args, size_t dim);

double rastigrin      (const double * args, size_t dim);

double rosenbrock     (const double * args, size_t dim);

double sphere         (const double * args, size_t dim);

double egghol2d       (const double * args, size_t dim);

double schaf2d        (const double * args, size_t dim);

double griewank       (const double * args, size_t dim);


/*******************************************************************************
  UTILITIES
******************************************************************************/

void print_solution(size_t dim, const double * solution);

double *filled_array(size_t length, double val);

#ifdef __cplusplus
}
#endif // __cplusplus