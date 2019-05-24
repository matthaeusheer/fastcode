# fastcode  [![Build Status](https://travis-ci.com/matthaeusheer/fastcode.svg?branch=master)](https://travis-ci.com/matthaeusheer/fastcode)

Group project for ETH course "How to Write Fast Numerical Code". The goal of this project was to implement 
nature inspired metaheuristic optimization algorithms with subsequent optimization of the performance.

Right now, 4 metaheuristic algorithms are implemented, namely  
- Grey Wolf Optimizer
- Particle Swarm Optimization
- Emperor Penguin Colony / Spiral Optimization fusion
- Squirrel Optimization

We focused on optimizing Particle Swarm Optimization mainly using SIMD vector instructions.

The **fastcode** repository contains two main components. The first one being the implementation of the algorithms 
in C and the timing infrastructure around them in C++. This code gets compiled into the *benchmark* executable which is 
responsible to run an algorithm with a specified user input for the parameters.  
Check out the different releases to obtain code for different steps along the code optimization process.
 
The second being the **fastpy** python package in the fastpy subfolder of fastcode. This code holds a wrapper around 
the C++ benchmarking code to conveniently run and benchmark large amounts of algorithm execution on any input and 
the subsequent output analysis / visualization and data management.

## Build instructions
We use ```cmake``` as our build tool. Make sure it is installed.

From within the project root do
```
mkdir build
cd build
cmake ..
make
```

---
**Note: DEBUG mode**


DEBUG mode: If you want to have the DEBUG preprocessor flag set to us
```
cmake -DDEBUG=ON ..
```
in the above example. (or, well, use an IDE which has Debug and Release modes, ha!)

---
---
**Note: Using different compilers**
Before invoking cmake, set the CC and CXX environment variables, i.e.  
```
export CC=icc
export CXX=icpc

mkdir build
cd build
cmake ..
make
```
to use intel compilers as an example.

---


This will compile all sources and link the targets for (currently, might get updated)
- benchmark (the main executable to run any algorithm on some user input and time this)
- executables for unit and integration tests

Hint: If you use an IDE such as CLion all of this will be handled automatically and you can bild individual
targets and run tests directly.

## Debugging

All debugging code should be wrapped in ifdef DEBUG, such as
```c
#ifdef DEBUG
  print_population(colony_size, dim, population);  // for evolution plot
  printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));  // for fitness plot
#endif
```

### Monitor single values
If you want to track one value over say all iterations (e.g. objective function value), 
put a  
```prtinf("# MY VALUE: %d", my_value);```  
inside your iteration loop. Then, parse lines which start with "# MY VALUE:".  
An example is given in the plot_optimization_evolution notebook.  

### Optimization evolution
To plot the 2D evolution of particles for an algorithm over time, 
check out the plot_optimization_evolution notebook. Data formats are described there. 
You basically need to use print_population in every iteration. Make sure not to 
plot a shitton of other stuff since then the parsing might fail. Lines starting with # will be handled, other stuff might break it.

### Other visualizations
There are lots of viualizations like roofline plots or performance metrics visualizations in the notebooks of the
fastpy package.

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
In the case of timing, all m repetitions are in one file, for solutions, there is a separate file for every repetition.

The file created by \-f is formatted like
```
iteration, cycles
0, 320299
1, 239529
2, 172374
... until (m - 1)  
```
The files created by \-o is formatted like 
```
dimension, value
0, -120
1, 98
... until (d - 1)  
```

### Evolution visualization and animation
His only works for 2D (d=2). Compile in debug mode (-DDEBUG=ON) and pipe the output 
of the benchmark executable to a file, e.g. parsed_output.txt.
Use the jupyter notebook of the fastpy package to read the piped output and 
produce the plot.

# fastPy
## Installation
1. Install the dependencies using pipenv
    ```
    cd fastpy
    pipenv install
    pipenv shell
    ```
2. Install the ipykernel for the pipenv virtual environment for jupyter
    ```
    python -m ipykernel install --user --name fastpy --display-name "fastpy"
    ```
    You can then chose this kernel in the jupyter notebook.

## Usage for benchmarking
1. Get the run config ready
    - In the fastpy folder there is a config_template.json file. Copy it and name the new 
    file config.json
    - All fields are lists and whatever values you put in the list, all possible 
    combinations of values will be performed
    - Every combination will be written out to a single output folder in the *data* folder.
    
2. Run ```python run_benchmark.py```.  
    This will perform benchmark runs as described above.
    Note: Run ```python run_benchmark.py --help``` to see cmd arguments.  
    --bin-dir has to match the directory inside the fastcode folder holding the benchmark 
    binary (defaults to "build")

    
## Analysis
Check out the various jupyter notebooks in the notebooks folder for 
- runtime plots
- performance plots
- roofline plots
- performance metrics plots
- algorithm visualization / animation
...
