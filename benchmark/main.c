#include <math.h>
#include <float.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"

void run_hgwosca(double(*obj)(const double* const, size_t),
                 size_t wolf_count,
                 size_t dim,
                 size_t max_iter,
                 double min_val,
                 double max_val) {

    double* mins = filled_array(dim, min_val);
    double* maxs = filled_array(dim, max_val);

    double* solution = hgwosca(sum_of_squares, wolf_count, dim, max_iter, mins, maxs);

    print_solution(dim, solution);
    free(solution);
    free(mins);
    free(maxs);
}


int main() {
    run_hgwosca(sum_of_squares, 30, 10, 120, -100, 100);
=
}
