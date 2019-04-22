/**************
BASIC PSO INTEGRATION TEST
***************/

#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "objectives.h"
#include "pso.h"
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define SWARM_SIZE 100
#define DIM 2

Test(pso_basic_integration, sum_of_squares) {
  size_t swarm_size = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 250;
  double* mins = filled_array(dim, -10);
  double* maxs = filled_array(dim, 10);
  double* solution = pso_basic(sum_of_squares, swarm_size, dim, max_iter, mins, maxs);

  printf("sum of sum_of_squares: %f\n", sum_of_squares(solution, dim) );
  printf("best sum_of_squares : \n"  );
	for (size_t d=0;d<dim;d++){
		printf(" sos %f \t", solution[d] );
	}

  cr_assert(fabs(sum_of_squares(solution, dim)) < 0.1, "sos objective should be minimised at 0");
}
/*
Test(pso_basic_integration, rosenbrock) {
  size_t swarm_count = 50;
  size_t dim = 5;
  size_t max_iter = 10000;
  double* mins = filled_array(dim, -100);
  double* maxs = filled_array(dim, 100);
  double* solution = pso_basic(rosenbrock, swarm_count, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] - 1) < 0.01, "each dimension should be reasonably close "
              "to 1");
  }
  free(solution);
  free(mins);
  free(maxs);
}
*/

Test(pso_basic_integration, sphere) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 250;
  double* mins = filled_array(dim, -5);
  double* maxs = filled_array(dim, 5);
  double* solution = pso_basic(sphere, swarm_count, dim, max_iter, mins, maxs);

  printf("sum of sphere:%f\n",sphere(solution, dim) );
  printf("best sphere : \n"  );
  for (size_t d=0;d<dim;d++){
    printf(" sph %f \t", solution[d] );
  }
  cr_assert(fabs(sphere(solution, dim) - 0 ) < 0.1, "sphere objective should be minimised at 0");
}

Test(pso_basic_integration, egghol2d) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = 2;
  size_t max_iter = 250;
  double* mins = filled_array(dim, -512);
  double* maxs = filled_array(dim, 512);
  double* solution = pso_basic(egghol2d, swarm_count, dim, max_iter, mins, maxs);

  printf("sum of egghol2d:%f\n",egghol2d(solution, dim) );
  printf("best egghol2d : \n"  );
  for (size_t d=0;d<dim;d++){
    printf(" egg %f \t", solution[d] );
  }
  cr_assert(fabs( egghol2d(solution, dim) - 959.6407 ) < 0.1, "egghol2d objective should be minimised at -959.6407");
}

Test(pso_basic_integration, rastigrin) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 250;
  double* mins = filled_array(dim, -5);
  double* maxs = filled_array(dim, 5);
  double* solution = pso_basic(rastigrin, swarm_count, dim, max_iter, mins, maxs);
  printf("sum of rastigrin:%f\n",rastigrin(solution, dim) );
  printf("best rastigrin : \n"  );
  for (size_t d=0;d<dim;d++){
    printf("ras %f \t", solution[d] );
  }
  cr_assert(fabs(rastigrin(solution, dim) - 0 ) < 0.1, "rastigrin objective should be minimised at 0");
}

/*
Test(pso_basic_integration, griewank) {
  size_t swarm_count = SWARM_SIZE;
  size_t dim = DIM;
  size_t max_iter = 250;
  double* mins = filled_array(dim, -5);
  double* maxs = filled_array(dim, 5);
  double* solution = pso_basic(griewank, swarm_count, dim, max_iter, mins, maxs);
  printf("sum of griewank:%f\n",griewank(solution, dim) );
  printf("best griewank : \n"  );
  for (size_t d=0;d<dim;d++){
    printf("gr %f \t", solution[d] );
  }
  cr_assert(fabs(griewank(solution, dim) - 0) < 0.1, "griewank objective should be minimised at 0");
}*/
