#include <criterion/criterion.h>

#include "float.h"
#include "math.h"
#include "utils.h"
#include "penguin.h"

Test(macro_tests, utilities) {
    cr_assert(0 == pen_min(0, 10), "minimum macro 1");
    cr_assert(-10 == pen_min(0.0, -10), "minimum macro 2");
    cr_assert(10 == pen_max(0, 10), "maximum macro 1");
    cr_assert(10 == pen_max(-INFINITY, 10), "maximum macro 2");
}


Test(penguin_unit, randomd) {
    srand((unsigned) time(NULL));

    double r = randomd(0.0, 1.0);
    cr_assert(r <= 1.0, "randomd upper bound 1");
    cr_assert(r >= 0.0, "randomd lower bound 1");

    r = randomd(-5.0, 0.0);
    cr_assert(r <= 0.0, "randomd upper bound 2");
    cr_assert(r >= -5.0, "randomd lower bound 2");

    r = randomd(-100.0, 100.0);
    cr_assert(r <= 100.0, "randomd upper bound 3");
    cr_assert(r >= -100.0, "randomd lower bound 3");

    r = randomd(0.0, 0.0);
    cr_assert(r <= 0.0, "randomd upper bound 4");
    cr_assert(r >= 0.0, "randomd lower bound 4");
}


Test(penguin_unit, generate_population) {
    srand((unsigned) time(NULL));

    double max[] = {100.0, 0.0, 11.0, -2.0};
    double min[] = {-100.0, -5.0, 10.0, -4.0};
    double* pop = pen_generate_population(100, 4, min, max);
    for(size_t idx = 0; idx < 4 * 100; idx += 4) {
        cr_assert(pop[idx] <= 100.0, "first dimension upper bound check");
        cr_assert(pop[idx] >= -100.0, "first dimension lower bound check");
        cr_assert(pop[idx + 1] <= 0.0, "second dimension upper bound check");
        cr_assert(pop[idx + 1] >= -5.0, "second dimension lower bound check");
        cr_assert(pop[idx + 2] <= 11.0, "third dimension upper bound check");
        cr_assert(pop[idx + 2] >= 10.0, "third dimension lower bound check");
        cr_assert(pop[idx + 3] <= -2.0, "fourth dimension upper bound check");
        cr_assert(pop[idx + 3] >= -4.0, "fourth dimension lower bound check");
    }
    free(pop);
}


Test(penguin_unit, copy_population) {
    srand((unsigned) time(NULL));

    double max[] = {100.0, 0.0, 11.0, -2.0};
    double min[] = {-100.0, -5.0, 10.0, -4.0};
    double* pop = pen_generate_population(100, 4, min, max);
    double* cpy = pen_copy_population(100, 4, pop);
    for(size_t idx = 0; idx < 4 * 100; idx++) {
        cr_assert(fabs(pop[idx] - cpy[idx]) < DBL_EPSILON, "copy and original are the same");
    }
    // TODO: Check that addresses are different.
    free(pop);
    free(cpy);
}


static double summation(const double* const args, size_t dim) {
    double sum = 0.0;
    for(size_t idx = 0; idx < dim; idx++) {
        sum += args[idx];
    }
    return sum;
}

// TODO: The messages get called when failing, so should be something like fitness 0 not correct.
Test(penguin_unit, get_initial_fitness) {
    double population[] = {1.0, 2.0, 3.0, 4.0, 5.0, -5.0, 10, -1.0};
    double* fitness = pen_get_initial_fitness(4, 2, population, summation);
    cr_assert(fabs(fitness[0] - 3.0) < DBL_EPSILON, "fitness 0 correct");
    cr_assert(fabs(fitness[1] - 7.0) < DBL_EPSILON, "fitness 1 correct");
    cr_assert(fabs(fitness[2] - 0.0) < DBL_EPSILON, "fitness 2 correct");
    cr_assert(fabs(fitness[3] - 9.0) < DBL_EPSILON, "fitness 3 correct");
    free(fitness);
}


Test(penguin_unit, eucledian_distance) {
    double point1[] = {10.0, 10.0};
    double point2[] = {10.0, 11.0};
    double dist = pen_eucledian_distance(2, point1, point2);
    cr_assert(fabs(dist - 1.0) < DBL_EPSILON, "distance test 1 passed");

    point1[0] = 0.0;
    point1[1] = 0.0;
    point2[0] = 3.0;
    point2[1] = 4.0;
    dist = pen_eucledian_distance(2, point1, point2);
    cr_assert(fabs(dist - 5.0) < DBL_EPSILON, "distance test 2 passed");
}


Test(penguin_unit, clamp_position) {
    double position[] = {-105, -90, 100, 1000, 0};
    double max[] = {100, 100, 100, 100, 100};
    double min[] = {-100, -100, -100, -100, -100};
    pen_clamp_position(5, position, min, max);
    cr_assert(fabs(position[0] + 100) < DBL_EPSILON, "first dimension clamped");
    cr_assert(fabs(position[1] + 90) < DBL_EPSILON, "second dimension clamped");
    cr_assert(fabs(position[2] - 100) < DBL_EPSILON, "third dimension clamped");
    cr_assert(fabs(position[3] - 100) < DBL_EPSILON, "fourth dimension clamped");
    cr_assert(fabs(position[4] - 0) < DBL_EPSILON, "fifth dimension clamped");
}


Test(penguin_unit, mutate) {
    double original[] = {0.0, 0.0, 0.0, 0.0};
    pen_mutate(4, original, 0.0);
    for(size_t idx = 0; idx < 4; idx++) {
        cr_assert(fabs(original[idx]) < DBL_EPSILON, "did not mutate");
    }

    pen_mutate(4, original, 1.0);
    for(size_t idx = 0; idx < 4; idx++) {
        cr_assert(fabs(original[idx]) < 1.0, "mutate did not overstep bounds");
    }

    original[0] = 10.0;
    original[1] = -10.0;
    original[2] = 0.0;
    original[3] = 0.5;
    pen_mutate(4, original, 0.5);

    cr_assert(fabs(original[0] - 10.0) < 0.5, "mutate did not overstep bounds, first dimension");
    cr_assert(fabs(original[1] + 10.0) < 0.5, "mutate did not overstep bounds, second dimension");
    cr_assert(fabs(original[2]) < 0.5, "mutate did not overstep bounds, third dimension");
    cr_assert(fabs(original[3] - 0.5) < 0.5, "mutate did not overstep bounds, fourth dimension");
}


Test(penguin_unit, attractiveness) {
    double penguin_i[] = {0.0, 0.0, 0.0, 0.0};
    double penguin_j[] = {0.0, 4.0, 3.0, 6.0};
    double penguin_k[] = {1.0, 1.0, 1.0, 0.0};
    double fitness[] = {100.0, 10.0, 9.0};

    double heat_rad_i = pen_heat_radiation(fitness[0]);
    double heat_rad_j = pen_heat_radiation(fitness[1]);
    double heat_rad_k = pen_heat_radiation(fitness[2]);

    cr_assert(heat_rad_i > heat_rad_j, "penguin i should radiate more than penguin j");
    cr_assert(heat_rad_i > heat_rad_k, "penguin i should radiate more than penguin k");
    cr_assert(heat_rad_j > heat_rad_k, "penguin j should radiate more than penguin k");

    double attractiveness_i_j = pen_attractiveness(heat_rad_i, 4, penguin_i, penguin_j, ATT_COEF_START);
    double attractiveness_j_i = pen_attractiveness(heat_rad_j, 4, penguin_i, penguin_j, ATT_COEF_START);
    cr_assert(attractiveness_i_j > attractiveness_j_i, "penguin j should be more attractive to "
    "penguin i than the other way around");
    double attractiveness_i_j_bis = pen_attractiveness(heat_rad_i, 4, penguin_j, penguin_i, ATT_COEF_START);
    cr_assert(fabs(attractiveness_i_j - attractiveness_i_j_bis) < DBL_EPSILON, "order of penguins in "
    "arguments should not matter");

    double attractiveness_i_k = pen_attractiveness(heat_rad_i, 4, penguin_i, penguin_k, ATT_COEF_START);
    cr_assert(attractiveness_i_j < attractiveness_i_k, "penguin k should be more attractive to "
    "penguin i than penguin j is attracted to penguin i, since k is closer to i");

    double attractiveness_j_k = pen_attractiveness(heat_rad_j, 4, penguin_j, penguin_k, ATT_COEF_START);
    cr_assert(attractiveness_i_j > attractiveness_j_k, "penguin j should be more attractive to "
    "penguin i than penguin k is attracted to penguin j, since i radiates much more "
    "energy");
}