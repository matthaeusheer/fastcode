#include <float.h>
#include <math.h>

#include "objectives.h"
#include "hgwosca.h"
#include "utils.h"

#include <criterion/criterion.h>


Test(hgwosca_unit, init_population) {
    size_t wolf_count = 40;
    size_t dim = 4;
    float min = 0.0;
    float max = 10.0;
    float population[wolf_count * dim];
    gwo_init_population(population, wolf_count, dim, min, max);
    for(size_t wolf = 0; wolf < wolf_count * dim; wolf++) {
        cr_assert(population[wolf] <= max, "dimension should be bound above");
        cr_assert(population[wolf] >= min, "dimension should be bound below");
    }
}


Test(hgwosca_unit, update_fitness) {
    size_t wolf_count = 4;
    size_t dim = 2;
    float population[] = {
            0.0, 0.0,
            1.0, 4.0,
            5.0, 0.0,
            2.0, 3.0
    };
    float fitness[] = {0.0, 0.0, 0.0, 0.0};
    gwo_update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
    cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
    cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
    cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
    cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, init_fitness) {
    size_t wolf_count = 4;
    size_t dim = 2;
    float population[] = {
            0.0, 0.0,
            1.0, 4.0,
            5.0, 0.0,
            2.0, 3.0
    };
    float fitness[wolf_count];
    gwo_init_fitness(fitness, wolf_count, dim, sum_of_squares, population);
    gwo_update_fitness(wolf_count, dim, sum_of_squares, population, fitness);
    cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first wolf's fitness should be 0");
    cr_assert(fabs(fitness[1] - 17.0) < DBL_EPSILON, "second wolf's fitness should be 17");
    cr_assert(fabs(fitness[2] - 25.0) < DBL_EPSILON, "third wolf's fitness should be 25");
    cr_assert(fabs(fitness[3] - 13.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");
}


Test(hgwosca_unit, update_leaders) {
    size_t wolf_count = 10;
    size_t alpha = 0, beta = 0, delta = 0;
    float fitness[] = {1.0, 2.0, 3.0, 3.0, 0.0, 5.0, 2.0, 1.0, 0.0, -1.0};
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    cr_assert(alpha == 9, "alpha should be 10th wolf");
    cr_assert((beta == 4 && delta == 8) || (delta == 4 && beta == 8), "beta and delta should "
    "be wolves 5 and 9, unordered");
    fitness[8] = 8.0;
    fitness[4] = -2.0;
    fitness[7] = 5.5;
    /* float fitness[] = {1.0, 2.0, 3.0, 3.0, -2.0, 5.0, 2.0, 5.5, 8.0, -1.0}; */
    gwo_update_leaders(wolf_count, fitness, &alpha, &beta, &delta);
    cr_assert(alpha == 4, "alpha should be 5th wolf");
    cr_assert(beta == 9, "beta should be 10th wolf");
    cr_assert(delta == 0, "delta should be 1st wolf");
}


Test(hgwosca_unit, clamp) {
    float vals[] = { -10.0, 0.0, 400, -3.0, 5.01};
    float min = 5.0;
    float max = 100;
    for(size_t idx = 0; idx < 5; idx++) {
        float res = gwo_clamp(vals[idx], min, max);
        cr_assert(res <= max, "clamped value is bound above by max");
        cr_assert(res >= min, "clamped value is bound below by min");
    }
}


Test(hgwosca_unit, clamp_all_positions) {
    float vals[] = {
            -10.0, 0.0, 400, -3.0, 5.01,
            45, 3, 30, -4, 5,
            -100, -100, -100, -100, 100
    };
    float min = 5.0;
    float max = 100;
    gwo_clamp_all_positions(3, 5, vals, min, max);
    for(size_t idx = 0; idx < 5; idx++) {
        cr_assert(vals[idx] <= max, "clamped value is bound above by max");
        cr_assert(vals[idx] >= min, "clamped value is bound below by min");
    }
}
