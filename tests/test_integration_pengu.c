#include <criterion/criterion.h>

#include <math.h>
#include "objectives.h"
#include "penguin.h"
#include "utils.h"


Test(pengu_integration, sum) {
  size_t population_size = 30;
  size_t dim = 2;
  size_t max_iter = 100;
  double* maxs = filled_array(dim, 100);
  double* mins = filled_array(dim, -100);
  double* solution = pen_emperor_penguin(sum, population_size, dim, max_iter, mins, maxs);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_assert(fabs(solution[idx] + 100) < 0.00001, "each dimension should be -100");
  }
  cr_assert(fabs(sum(solution, dim) + 100 * dim) < 0.2 * dim, "objective should be "
                                                              "minimised at -100*dim");
  free(solution);
  free(mins);
  free(maxs);
}