# Benchmark

## Algorithm Benchmarks

In order to perform algorithm benchmarks, `make` to build the executable and then use
it:

```
Usage: bench [-van]
  -v    verbose
  -a    algorithm name
  -n    number of iterations to run (default=2048)

  If no algorithm name is given, the operation
  benchmark is performed.

  Example:
    bench -a hgwosca -n 4048
    will run the HGWOSCA algorithm for 4048
    iterations. No verbose output will be provided.
```

This usage information is also shown using `bench -h`.

## Operation Benchmarks

The tool built in the section above also supports benchmarking of individual operations.
In order to launch such a benchmark, simply add a line such as:

```
BENCH_OP(name, expression)
```

in `ops.c`. Ensure the `name` is a single identifier-like token. The `expression` can
operate on up to three operands (`x[i]`, `y[i]`, and `z[i]`). If more operands are needed,
add a new one using the `build()` method and ensure you free the memory again at the end
of `oepration_benchmark()`.

Then simply recompile using `make` and launch `bench` without any extra arguments to
obtain the benchmarks.

### Seg Faults

If running the code gives a segmentation fault, try increasing `ARRAY_SIZE` in `utils.h`.
This variable defines the size of the arrays that store the data on computations. If
these arrays are too small for the number of iterations, it results in a segmentation fault.

### Minimum Runs

Use the `NUM_RUNS` variable to control the minimum number of runs to perform. Note this is only
useful if `expression` takes longer than `CYCLES_REQUIRED` cycles. This should never be the case
unless `expression` contains a very computationally intensive function call.

### Sample Output

```
addition:       1.96243 	cycles
multiplication:	1.98121 	cycles
division:       11.29555 	cycles
three_oper:     2.51969 	cycles
square_root:    16.16629 	cycles
```
