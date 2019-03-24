#pragma once

double* const hgwosca(double(*obj)(const double* const, size_t),
                      size_t wolf_count,
                      size_t dim,
                      size_t max_iterations,
                      const double* const min_positions,
                      const double* const max_positions);
