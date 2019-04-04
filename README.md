# fastcode  [![Build Status](https://travis-ci.com/matthaeusheer/fastcode.svg?branch=master)](https://travis-ci.com/matthaeusheer/fastcode)

Group project for ETH course "How to Write Fast Numerical Code".

## Testing

This project uses [`criterion`](https://github.com/Snaipe/Criterion) for testing.  

### Run all tests
```make test``` <- compiles and runs all tests.

### Guidelines
Tests should be written in te **tests/** folder.  

Integration tests, for now, are written into test_integration.c.  
Unit tests get their own file, i.e. benchmark.c will be tested in test_benchmark.c.

The tests are compiled all together to the bin directory using  ```make test```.
This target also runs the tests directly and is the default target as well.

In the **future**: We will probably have one integration test executable per algorithm and 
separate executables for the unit tests. Right now all of them get compiled into one executable.


### Installation

`criterion` can be installed on debian based distributions using:

```
sudo add-apt-repository ppa:snaipewastaken/ppa
sudo apt-get update
sudo apt-get install criterion-dev
```

## Benchmarking

To **compile** the benchmark executable run:  
 ```make benchmark```.

To  **run** the benchmark wrapper:
1) Go to fastpy directory: ```cd fastpy```
2) Follow the instructions given in the README.md file.