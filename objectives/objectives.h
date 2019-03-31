#pragma once

#include <functional>
#include <map>
#include <string>

typedef double (*obj_func_t) (const double* const, size_t);
typedef std::map<std::string, obj_func_t> obj_map_t;


/*******************************************************************************
  OBJECTIVE FUNCTIONS PROTOTYPES
******************************************************************************/

double sum_of_squares   (const double* const args, size_t dim);
double sum              (const double* const args, size_t dim);
double rastigrin        (const double* const args, size_t dim);
double rosenbrock       (const double* const args, size_t dim);
double sphere           (const double* const args, size_t dim);
double egghol2d         (const double* const args, size_t dim);
double schaf2d          (const double* const args, size_t dim);
double griewank         (const double* const args, size_t dim);


/*******************************************************************************
  UTILITIES
******************************************************************************/

void print_solution(size_t dim, const double* const solution);
double* filled_array(size_t length, double val);
obj_map_t create_obj_map();
