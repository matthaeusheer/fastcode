#include <float.h>
#include <math.h>

#include "objectives.h"
#include "hgwosca.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(hgwosca_unit, init_population) {
    size_t wolf_count = 40;
    size_t dim = 4;
    double mins[] = {0.0, 4.0, 0.001, 0.3};
    double maxs[] = {10.0, 4.0, 0.002, 0.6};
    double* const population = gwo_init_population(wolf_count, dim, mins, maxs);
    for(size_t wolf = 0; wolf < wolf_count; wolf++) {
        cr_assert(population[wolf * dim] <= maxs[0], "first dimension should be bound above");
        cr_assert(population[wolf * dim] >= mins[0], "first dimension should be bound below");
        cr_assert(population[wolf * dim + 1] <= maxs[1], "second dimension should be bound above");
        cr_assert(population[wolf * dim + 1] >= mins[1], "second dimension should be bound below");
        cr_assert(population[wolf * dim + 2] <= maxs[2], "third dimension should be bound above");
        cr_assert(population[wolf * dim + 2] >= mins[2], "third dimension should be bound below");
        cr_assert(population[wolf * dim + 3] <= maxs[3], "fourth dimension should be bound above");
        cr_assert(population[wolf * dim + 3] >= mins[3], "fourth dimension should be bound below");
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
    cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
    cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
    cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
    cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
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
    double* const fitness = gwo_init_fitness(wolf_count, dim, sum_of_squares, population);
    gwo_update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
    cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
    cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
    cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
    cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, update_leaders) {
    size_t wolf_count = 10;
    size_t alpha = 0, beta = 0, delta = 0;
    double fitness[] = {1.0, 2.0, 3.0, 3.0, 0.0, 5.0, 2.0, 1.0, 0.0, -1.0};
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    cr_assert(alpha == 9, "alpha should be 10th wolf");
    cr_assert((beta == 4 && delta == 8) || (delta == 4 && beta == 8), "beta and delta should "
    "be wolves 5 and 9, unordered");
    fitness[8] = 8.0;
    fitness[4] = -2.0;
    fitness[7] = 5.5;
    /* double fitness[] = {1.0, 2.0, 3.0, 3.0, -2.0, 5.0, 2.0, 5.5, 8.0, -1.0}; */
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    cr_assert(alpha == 4, "alpha should be 5th wolf");
    cr_assert(beta == 9, "beta should be 10th wolf");
    cr_assert(delta == 0, "delta should be 1st wolf");
}


Test(hgwosca_unit, clamp) {
    double vals[] = { -10.0, 0.0, 400, -3.0, 5.01};
    double mins[] = { 5.0, -5.0, 30, -10, 0};
    double maxs[] = { 100, 1.0, 30, -4, 5};
    for(size_t idx = 0; idx < 5; idx++) {
        double res = gwo_clamp(vals[idx], mins[idx], maxs[idx]);
        cr_assert(res <= maxs[idx], "clamped value is bound above by max");
        cr_assert(res >= mins[idx], "clamped value is bound below by min");
    }
}


Test(hgwosca_unit, clamp_all_positions) {
    double vals[] = {
            -10.0, 0.0, 400, -3.0, 5.01,
            45, 3, 30, -4, 5,
            -100, -100, -100, -100, 100
    };
    double mins[] = { 5.0, -5.0, 30, -10, 0};
    double maxs[] = { 100, 1.0, 30, -4, 5};
    gwo_clamp_all_positions(3, 5, vals, mins, maxs);
    for(size_t idx = 0; idx < 5; idx++) {
        cr_assert(vals[idx] <= maxs[idx], "clamped value is bound above by max");
        cr_assert(vals[idx] >= mins[idx], "clamped value is bound below by min");
    }
}
