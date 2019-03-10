# Operation Benchmark

This code can be used to measure the cycle count of operations.

## Adding a Test

In order to add a measure, add the a line:

```
BENCH_OP(name, expression)
```

in the `main()` function. Ensure the `name` is a single identifier-like token. The `expression` can
operate on up to three operands (`x[i]`, `y[i]`, and `z[i]`). If more operands are needed, add
a new one using the `build()` method and ensure you free the memory again at the end of `main()`.

## Compiling

Simply compile `bench.c` with the desired flags. For instance:

```
gcc-8 -O3 -fno-tree-vectorize bench.c -o bench
```

## Seg Faults

If running the code gives a segmentation fault, try increasing `N` in the code. This variable
defines the size of the arrays that store the data on computations. If these arrays are too small
for the number of iterations, it results in a segmentation fault.

## Minimum Runs

Use the `NUM_RUNS` variable to control the minimum number of runs to perform. Note this is only
useful if `expression` takes longer than `CYCLES_REQUIRED` cycles. This should never be the case
unless `expression` contains a very computationally intensive function call.

## Sample Output

```
addition:       1.96243 	cycles
multiplication:	1.98121 	cycles
division:       11.29555 	cycles
three_oper:     2.51969 	cycles
square_root:    16.16629 	cycles
```
