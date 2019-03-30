#ifndef WIN32
#include <sys/time.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tsc_x86.h"

#include "utils.h"

#define CYCLES_REQUIRED 1e7
#define NUM_RUNS 1

#define BENCH_OP(name, expr)                    \
    num_runs = NUM_RUNS;                        \
    multiplier = 1.0;                           \
    do {                                        \
        num_runs = num_runs * multiplier;       \
        start = start_tsc();                    \
        for(i = 0; i < num_runs; ++i) {         \
            x[i] = expr;                        \
        }                                       \
        end = stop_tsc(start);                  \
        cycles = (double)end;                   \
        multiplier = CYCLES_REQUIRED / cycles;  \
    } while(multiplier > 2);                    \
    start = start_tsc();                        \
    for(i = 0; i < num_runs; ++i) {             \
        x[i] = expr;                            \
    }                                           \
    end = stop_tsc(start);                      \
    cycles = (double)end / num_runs;            \
    printf("%s:\t", #name);                     \
    printf("%.5f \tcycles\n", cycles);


/**
   Perform benchmarks on predefined operations.
*/
void operation_benchmark() {
  srand(0);
  double* x = build();
  double* y = build();
  double* z = build();
  long i, num_runs;
  double cycles, multiplier;
  myInt64 start, end;

  // Define benchmarks here
  BENCH_OP(addition, x[i] + y[i])
    BENCH_OP(multiplication, x[i] * y[i])
    BENCH_OP(division, x[i] / y[i])
    BENCH_OP(three_oper, x[i] + y[i] + z[i])
    BENCH_OP(square_root, sqrt(x[i]))

    free(x);
  free(y);
  free(z);
}
