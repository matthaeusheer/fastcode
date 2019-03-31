#pragma once


void run_hgwosca(double(*obj)(const double* const, size_t),
                 size_t wolf_count,
                 size_t dim,
                 size_t max_iter,
                 double min_val,
                 double max_val,
                 bool print_out);