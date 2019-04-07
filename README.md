# fastcode  [![Build Status](https://travis-ci.com/matthaeusheer/fastcode.svg?branch=master)](https://travis-ci.com/matthaeusheer/fastcode)

Group project for ETH course "How to Write Fast Numerical Code".

## How to Build
We use ```cmake``` as our build tool. Make sure it is installed.

From within the project root do
```
mkdir build
cd build
cmake ..
make
```
This will compile all sources and link the targets for (currently, might get updated)
- benchmark (main executable to run the algorithms on some input and time them)
- test_units (collection of all unit tests)
- test_integration (integration tests)
- test_<unit_test_name> (single unit tests)

You can later on build individual targets by make <target_name>. Check CMakeLists for available targets.

Hint: If you use an IDE such as CLion all of this will be handled automatically and you can bild individual
targets and run tests directly.

## Testing

This project uses [`criterion`](https://github.com/Snaipe/Criterion) for testing.  


### Guidelines
Tests should be written in te **tests/** folder.  

Integration tests, for now, are written into test_integration.c.  
Unit tests get their own file, i.e. benchmark.c will be tested in test_benchmark.c.


### Installation

`criterion` can be installed on debian based distributions using:

```
sudo add-apt-repository ppa:snaipewastaken/ppa
sudo apt-get update
sudo apt-get install criterion-dev
```

## Benchmarking

After compiling the **benchmark** executable you can either
- run the executable directly from the command line  
(no command line arguments shows usage)  

or

- run the **benchmark wrapper**:
  1) Go to fastpy directory: ```cd fastpy```
  2) Follow the instructions given in the README.md file  

### Benchmark usage
You can run any algorithm on any objective function on any input data.

```
Usage:  [-vaofsnmpyz]"                              
  -v    verbose"                                      
  -a    algorithm name"                               
  -o    objective function name"                      
  -f    output timing file name"                      
  -s    output solution file name"                    
  -n    number of iterations to run per algorithm"    
  -m    number of repetitions of an algorihtm"        
  -p    population size"                              
  -y    minimum values"                               
  -z    maximum values"                               
                                                 
Currently all parameters are required.             
Example:                                             
    ./benchmark -a "hgwosca" -o "sum" -n 50 -m 1 -d 10 -p 30 -y -120 -z 100 -s "../data/solution.txt" -f "../data/timings.txt" 
```
-f and -o parameters take absolute and relative paths.  
For a combination of parameters / algorithms / objective functions, see the python wrapper.

### Benchmark Output
\-f and \-o parameters specify the output files for timing and solution logging respectively.

The file created by \-f is formatted like
```
iteration, cycles
0, 320299
1, 239529
2, 172374
... until (n - 1)  
```
The files created by \-o is formatted like 
```
dimension, value
0, -120
1, 98
... until (d - 1)  
```