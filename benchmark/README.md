# Benchmark

## Algorithm Benchmarks

In order to perform algorithm benchmarks, `make` to build the executable __benchmark__ and then use
it as follows:

```
Usage: benchmark [-vaofsnpyz]                         
   -v    verbose                                   
   -a    algorithm name                            
   -o    objective function name                   
   -f    output timing file name                   
   -s    output solution file name                 
   -n    number of iterations to run               
   -p    population size                  
   -y    minimum values                            
   -z    maximum values                            
                                               
   Example:                                        
     ./benchmark -a "hgwosca" -o "sum" -n 3 -d 10 -p 30 -y -120 -z 100 -s "solution.txt" -f "timings.txt"                            \
```
\-f and \-o parameters take absolute/relative paths as well.

This usage information is also shown using `bench -h`.

## Algorithm Benchmarks
This executable runs our algorithms and measures the cycles performed for them  
to complete for a given parameter set. All algorithms and objective functions which are registered in 
__benchmark.cpp__ are runnable. Parameters are handed in via command line.

### Output
\-f and \-o parameters specify the output files for timing and solution logging respectively.

The file created by \-f is formatted like
```
iteration, cycles
0, 320299
1, 239529
2, 172374
... until (n - 1)  
```
The file created by \-o is formatted like 
```
dimension, value
0, -120
1, 98
... until (d - 1)  
```

## Operation Benchmarks

__TODO: CURRENTLY THIS SECTION IS NOT TRUE, SORRY JAKOB :)__

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
