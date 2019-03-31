#include <math.h>
#include <float.h>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include <vector>

#include <criterion/criterion.h>

#include "../objectives/objectives.h"
#include "../HGWOSCA/main.h"
#include "../operation-bench/tsc_x86.h"
#include "utils.hpp"

void run_hgwosca(double(*obj)(const double* const, size_t),
                 size_t wolf_count,
                 size_t dim,
                 size_t max_iter,
                 double min_val,
                 double max_val,
                 bool print_out) {

    double* mins = filled_array(dim, min_val);
    double* maxs = filled_array(dim, max_val);

    double* solution = hgwosca(sum_of_squares, wolf_count, dim, max_iter, mins, maxs);

    if (print_out)
        print_solution(dim, solution);

    free(solution);
    free(mins);
    free(maxs);
}


int main() {

    for(auto const& dim: range<int, 100, 1000, 100>()) {
        std::cout << "Input size: " << dim << std::endl;

        myInt64 start_time = start_tsc();
        run_hgwosca(sum_of_squares, 30, dim, 120, -100, 100, false);
        myInt64 stop_time = stop_tsc(start_time);

        printf("Run function in %llu cycles.\n", stop_time);

    }

}
