#include <criterion/criterion.h>

#include "float.h"
#include "math.h"
#include "objectives.h"
#include "penguin.h"

Test(macro_tests, min_and_max) {
  cr_assert(0 == pen_min(0, 10), "minimum macro 1");
  cr_assert(-10 == pen_min(0.0, -10), "minimum macro 2");
  cr_assert(10 == pen_max(0, 10), "maximum macro 1");
  cr_assert(10 == pen_max(-INFINITY, 10), "maximum macro 2");
}


Test(penguin_unit, initialise_population) {
  // Generating the max and min values allowed for every dimension.
  double max = 100.0;
  double min = -100.0;

  double population[100 * 4];

  // Generating population of size 100 with dim 4.
  pen_initialise_population(population, 100, 4, min, max);

  // Checking if the initial values of the penguin population are within the specified bounds.
  for (size_t idx = 0; idx < 4 * 100; idx++) {
    cr_assert(population[idx] <= 100.0, "dimension should be upper bound");
    cr_assert(population[idx] >= -100.0, "dimension should be lower bound");
  }
}


Test(penguin_unit, get_initial_fitness) {
  double population[] = {1.0, 2.0, 3.0, 4.0, 5.0, -5.0, 10, -1.0};

  double fitness[4];
  // Getting the initial fitness of population of size 4 and dim 2 by using the summation function.
  pen_update_fitness(fitness, 4, 2, population, sum);

  cr_assert_eq(fitness[0], 3.0, "Fitness of penguin 0 incorrect.");
  cr_assert_eq(fitness[1], 7.0, "Fitness of penguin 1 incorrect.");
  cr_assert_eq(fitness[2], 0.0, "Fitness of penguin 2 incorrect.");
  cr_assert_eq(fitness[3], 9.0, "Fitness of penguin 3 incorrect.");

}


Test(penguin_unit, eucledian_distance) {
  double point1[] = {10.0, 10.0};
  double point2[] = {10.0, 11.0};
  double dist = pen_eucledian_distance(2, point1, point2);
  cr_assert_eq(dist, 1.0, "Distance test 1 failed.");

  point1[0] = 0.0;
  point1[1] = 0.0;
  point2[0] = 3.0;
  point2[1] = 4.0;
  dist = pen_eucledian_distance(2, point1, point2);
  cr_assert_eq(dist, 5.0, "Distance test 2 failed.");
}


Test(penguin_unit, clamp_position) {
  double position[] = {-105, -90,   100,  1000, 0  };
  double max = 100;
  double min = -100;
  pen_clamp_position(5, position, min, max);

  cr_assert_eq(position[0], -100, "1st dimension clamp failed.");
  cr_assert_eq(position[1], -90,  "2nd dimension clamp failed.");
  cr_assert_eq(position[2], 100,  "3rd dimension clamp failed.");
  cr_assert_eq(position[3], 100,  "4th dimension clamp failed.");
  cr_assert_eq(position[4], 0,    "5th dimension clamp failed.");
}


Test(penguin_unit, mutate) {
  double original[] = {0.0, 0.0, 0.0, 0.0};
  size_t dim = 4;

  // Test whether permutation happens even though mutation_coef = 0.
  double mutation_coef = 0.0;
  double expected[] = {0.0, 0.0, 0.0, 0.0};
  pen_mutate(dim, original, mutation_coef);
  cr_assert_arr_eq(original, expected, dim, "For mutation coef zero, no mutation should happen.");

  // Test whether permuted values stay in range of expected values.
  mutation_coef = 1.0;
  pen_mutate(dim, original, 1.0);
  for (size_t idx = 0; idx < 4; idx++) {
    cr_assert_gt(original[idx], -1.0 * mutation_coef, "Permuted value should be larger than lower bound.");
    cr_assert_lt(original[idx], 1.0 * mutation_coef, "Permuted value should be smaller than upper bound.");
  }

  // Same test as before for more complex scenario.
  original[0] = 10.0;
  original[1] = -10.0;
  original[2] = 0.0;
  original[3] = 0.5;
  mutation_coef = 0.5;
  pen_mutate(dim, original, mutation_coef);

  cr_assert_lt(fabs(original[0] - 10.0), 0.5, "First dimension out of bounds.");
  cr_assert_lt(fabs(original[1] + 10.0), 0.5, "Second dimension out of bounds.");
  cr_assert_lt(fabs(original[2] - 0.0), 0.5, "Third dimension out of bounds.");
  cr_assert_lt(fabs(original[3] - 0.5), 0.5, "Fourth dimension out of bounds.");
}


Test(penguin_unit, attractiveness) {
  size_t dim = 2;
  double penguins[] = {
    0.0, 0.0,
    4.0, 4.0,
    2.0, 1.0
  };
  double heat_rad = 2.3;
  cr_assert_eq(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
               pen_attractiveness(heat_rad, dim, &penguins[2], &penguins[0], 0.5),
               "attractiveness should not be affected by the order of the penguins");
  cr_assert_lt(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
               pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[4], 0.5),
               "penguins 1 should attract penguin 3 more than it attracts penguin 2");
  cr_assert_eq(pen_attractiveness(0, dim, &penguins[0], &penguins[2], 0.5),
               0.0,
               "attractiveness should linearly scale with heat radiation");
  cr_assert_lt(pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.5),
               pen_attractiveness(heat_rad, dim, &penguins[0], &penguins[2], 0.1),
               "attractiveness should scale inverse exponentially with attenuation \
                coefficient");
}
