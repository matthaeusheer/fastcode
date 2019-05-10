#include <criterion/criterion.h>

#include "float.h"
#include "math.h"
#include "objectives.h"
#include "penguin.h"

Test(macro_tests, min_and_max) {
  cr_expect_eq(0, pen_min(0, 10), "minimum macro 1");
  cr_expect_eq(-10, pen_min(0.0, -10), "minimum macro 2");
  cr_expect_eq(10, pen_max(0, 10), "maximum macro 1");
  cr_expect_eq(10, pen_max(-INFINITY, 10), "maximum macro 2");
}


Test(penguin_unit, initialise_population) {
  float max = 100.0;
  float min = -100.0;
  float population[100 * 4];
  pen_initialise_population(population, 100, 4, min, max);
  for (size_t idx = 0; idx < 4 * 100; idx++) {
    cr_expect_leq(population[idx], 100.0,
                  "dimension of penguin %ld should be upper bound", idx);
    cr_expect_geq(population[idx], -100.0,
                  "dimension of penguin %ld should be lower bound", idx);
  }
}


Test(penguin_unit, get_initial_fitness) {
  float population[] = {1.0, 2.0, 3.0, 4.0, 5.0, -5.0, 10, -1.0};
  float fitness[4];
  pen_update_fitness(fitness, 4, 2, population, sum);
  cr_expect_float_eq(fitness[0], 3.0, 0.001, "fitness of penguin 0 incorrect");
  cr_expect_float_eq(fitness[1], 7.0, 0.001, "fitness of penguin 1 incorrect");
  cr_expect_float_eq(fitness[2], 0.0, 0.001, "fitness of penguin 2 incorrect");
  cr_expect_float_eq(fitness[3], 9.0, 0.001, "fitness of penguin 3 incorrect");

}


Test(penguin_unit, eucledian_distance) {
  float point1[] = {10.0, 10.0};
  float point2[] = {10.0, 11.0};
  float dist = pen_eucledian_distance(2, point1, point2);
  cr_expect_float_eq(dist, 1.0, 0.001, "distance test 1 failed");
  point1[0] = 0.0;
  point1[1] = 0.0;
  point2[0] = 3.0;
  point2[1] = 4.0;
  dist = pen_eucledian_distance(2, point1, point2);
  cr_expect_float_eq(dist, 5.0, 0.001, "distance test 2 failed");
}


Test(penguin_unit, clamp_position) {
  float position[] = {-105, -90,   100,  1000, 0  };
  float max = 100;
  float min = -100;
  pen_clamp_position(5, position, min, max);
  cr_expect_float_eq(position[0], -100, 0.001, "1st dimension clamp failed");
  cr_expect_float_eq(position[1], -90, 0.001, "2nd dimension clamp failed");
  cr_expect_float_eq(position[2], 100, 0.001, "3rd dimension clamp failed");
  cr_expect_float_eq(position[3], 100, 0.001, "4th dimension clamp failed");
  cr_expect_float_eq(position[4], 0, 0.001, "5th dimension clamp failed");
}


Test(penguin_unit, mutate) {
  float original[] = {0.0, 0.0, 0.0, 0.0};
  size_t dim = 4;
  float mutation_coef = 0.0;
  float expected[] = {0.0, 0.0, 0.0, 0.0};
  pen_mutate(dim, original, mutation_coef);
  for(size_t idx = 0; idx < dim; idx++) {
    cr_expect_float_eq(original[idx], expected[idx], 0.001,
                       "no mutation should happen at index %ld", idx);
  }
  mutation_coef = 1.0;
  pen_mutate(dim, original, 1.0);
  for (size_t idx = 0; idx < 4; idx++) {
    cr_expect_gt(original[idx], -1.0 * mutation_coef,
                 "permuted value should be lower bound at index %ld", idx);
    cr_expect_lt(original[idx], 1.0 * mutation_coef,
                 "permuted value should be upper bound at index %ld", idx);
  }

  original[0] = 10.0;
  original[1] = -10.0;
  original[2] = 0.0;
  original[3] = 0.5;
  mutation_coef = 0.5;
  pen_mutate(dim, original, mutation_coef);
  cr_expect_float_eq(original[0], 10.0, 0.5, "first dimension out of bounds");
  cr_expect_float_eq(original[1], -10.0, 0.5, "second dimension out of bounds");
  cr_expect_float_eq(original[2], 0.0, 0.5, "third dimension out of bounds");
  cr_expect_float_eq(original[3], 0.5, 0.5, "fourth dimension out of bounds");
}


Test(penguin_unit, attractiveness) {
  size_t dim = 2;
  float penguins[] = {
    0.0, 0.0,
    4.0, 4.0,
    2.0, 1.0
  };
  float heat_rad = 2.3;
  cr_expect_float_eq(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
                     pen_attractiveness(heat_rad, dim, &penguins[2], &penguins[0], 0.5),
                     0.001,
                     "attractiveness should not be affected by the order of the penguins");
  cr_expect_lt(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
               pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[4], 0.5),
               "penguins 1 should attract penguin 3 more than it attracts penguin 2");
  cr_expect_float_eq(pen_attractiveness(0, dim, &penguins[0], &penguins[2], 0.5),
                     0.0,
                     0.001,
                     "attractiveness should linearly scale with heat radiation");
  cr_expect_lt(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
               pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.1),
               "attractiveness should scale inverse exponentially with attenuation \
                coefficient");
}
