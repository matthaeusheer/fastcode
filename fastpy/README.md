# The **_fastpy_** package

## Goals
This python package provides functionality to perform automated benchmark runs 
on the belonging _fastcode_ C/C++ algorithms with subsequent performance analysis. 

This is a standalone python package which in principle only needs the _./benchmark_ executable
which it detects automatically inside the encompassing _fastcode_ C package.

## How-To

### Run Benchmark runs
1) Copy the provided config template and modify it for your needs.
    ```bash
    cp config_template.json config.json
    ```
    Now edit the ```config.json``` file.

2) Run the benchmark wrapper script.
    ```bash
    ./run_benchmark.py config.json
    ```
    This runs the benchmark wrapper script using the config.json configuration.

### Output
Output files will be written to the ```data``` folder by default.  
For the complete benchmark run one output folder will be created tagged by datetime holding
- config.json
- binary executable

Inside this folder, each parameter combination run will have its own output directory holding
- run_config.json
- timings.txt
- solution_.txt, one file for the _last_ iteration - we can change this soon to hold all solutions.

Timings and solution file format are described in the C/C++ _fastcode_ benchmark module README.

### Evalution

Coming soon.