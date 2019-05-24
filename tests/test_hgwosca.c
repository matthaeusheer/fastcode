#include <float.h>
#include <math.h>

#include "objectives.h"
#include "hgwosca.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(hgwosca_unit, init_population) {
  size_t wolf_count = 40;
  size_t dim = 4;
  double min = 0.0;
  double max = 10.0;
  double population[wolf_count * dim];
  gwo_init_population(population, wolf_count, dim, min, max);
  for(size_t wolf = 0; wolf < wolf_count * dim; wolf++) {
    cr_expect_leq(population[wolf], max,
                  "dimension of wolf %ld should be bound above", wolf);
    cr_expect_geq(population[wolf], min,
                  "dimension of wolf %ld should be bound below", wolf);
  }
}


Test(hgwosca_unit, update_fitness) {
  size_t wolf_count = 4;
  size_t dim = 2;
  double population[] = {
      0.0, 0.0,
      1.0, 4.0,
      5.0, 0.0,
      2.0, 3.0
  };
  double fitness[] = {0.0, 0.0, 0.0, 0.0};
  gwo_update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
  cr_expect_float_eq(fitness[0], 0.0, FLT_EPSILON, "first wolf's fitness should be 0");
  cr_expect_float_eq(fitness[1], 17.0, FLT_EPSILON, "second wolf's fitness should be 17");
  cr_expect_float_eq(fitness[2], 25.0, FLT_EPSILON, "third wolf's fitness should be 25");
  cr_expect_float_eq(fitness[3], 13.0, FLT_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, init_fitness) {
  size_t wolf_count = 4;
  size_t dim = 2;
  double population[] = {
      0.0, 0.0,
      1.0, 4.0,
      5.0, 0.0,
      2.0, 3.0
  };
  double fitness[wolf_count];
  gwo_init_fitness(fitness, wolf_count, dim, sum_of_squares, population);
  gwo_update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
  cr_expect_float_eq(fitness[0], 0.0, FLT_EPSILON, "first wolf's fitness should be 0");
  cr_expect_float_eq(fitness[1], 17.0, FLT_EPSILON, "second wolf's fitness should be 17");
  cr_expect_float_eq(fitness[2], 25.0, FLT_EPSILON, "third wolf's fitness should be 25");
  cr_expect_float_eq(fitness[3], 13.0, FLT_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, update_leaders) {
  size_t wolf_count = 10;
  size_t alpha = 0, beta = 0, delta = 0;
  double fitness[] = {1.0, 2.0, 3.0, 3.0, 0.0, 5.0, 2.0, 1.0, 0.0, -1.0};
  gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  cr_expect_eq(alpha, 9, "alpha should be 10th wolf");
  cr_expect((beta == 4 && delta == 8) || (delta == 4 && beta == 8),
            "beta and delta should be wolves 5 and 9, unordered");
  fitness[8] = 8.0;
  fitness[4] = -2.0;
  fitness[7] = 5.5;
  /* double fitness[] = {1.0, 2.0, 3.0, 3.0, -2.0, 5.0, 2.0, 5.5, 8.0, -1.0}; */
  gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
  cr_expect_eq(alpha, 4, "alpha should be 5th wolf");
  cr_expect_eq(beta, 9, "beta should be 10th wolf");
  cr_expect_eq(delta, 0, "delta should be 1st wolf");
}


Test(hgwosca_unit, clamp) {
  double vals[] = { -10.0, 0.0, 400, -3.0, 5.01};
  double min = 5.0;
  double max = 100;
  for(size_t idx = 0; idx < 5; idx++) {
    double res = gwo_clamp(vals[idx], min, max);
    cr_expect_leq(res, max,
                  "clamped value at index %ld is bound above by max", idx);
    cr_expect_geq(res, min,
                  "clamped value at index %ld is bound below by min", idx);
  }
}


Test(hgwosca_unit, clamp_all_positions) {
  double vals[] = {
      -10.0, 0.0, 400, -3.0, 5.01,
      45, 3, 30, -4, 5,
      -100, -100, -100, -100, 100
  };
  double min = 5.0;
  double max = 100;
  gwo_clamp_all_positions(3, 5, vals, min, max);
  for(size_t idx = 0; idx < 5; idx++) {
    cr_expect_leq(vals[idx], max,
                  "clamped value at index %ld is bound above by max", idx);
    cr_expect_geq(vals[idx], min,
                  "clamped value at index %ld is bound below by min", idx);
  }
}
