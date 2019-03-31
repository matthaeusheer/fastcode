#include <cstdlib>

#include "run_algos.h"
#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"

void run_hgwosca(double(*obj_func)(const double* const, size_t),
                 size_t wolf_count,
                 size_t dim,
                 size_t max_iter,
                 double min_val,
                 double max_val,
                 bool print_out) {

    double* mins = filled_array(dim, min_val);
    double* maxs = filled_array(dim, max_val);

    double* solution = hgwosca(obj_func, wolf_count, dim, max_iter, mins, maxs);

    if (print_out)
        print_solution(dim, solution);

    free(solution);
    free(mins);
    free(maxs);
}