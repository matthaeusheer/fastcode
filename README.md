# fastcode  [![Build Status](https://travis-ci.com/matthaeusheer/fastcode.svg?branch=master)](https://travis-ci.com/matthaeusheer/fastcode)

Group project for ETH course "How to Write Fast Numerical Code".

## Testing

This project uses [`criterion`](https://github.com/Snaipe/Criterion) for testing. Unit
tests should be written straight in the `main.c` file of the algorithm. Integration tests
such as numerical validations can be written in the `tests/integration.c` file. In order to
run all tests (unit and integration), please `make` all algorithms, then use:

```
make
make test
```

inside the `tests` directory. Please also list your main object file in the `ALGOS` argument
in `tests/Makefile`.

The same can be performed in the project directories as well to only run the unit tests for said
project.


### Running All Tests

The command:

```
./test_all.sh
```

can be used to compile all registered projects and test the registered tests (both unit and
integration).

In order to register a project, add the directory name in the shell script and ensure the main
file of the project is called `main.c`. Moreover, the main object file `main.o` might have to
be imported in `ALGOS` in the `tests/Makefile`.

In order to implement integration tests, register the project as explained above and simply
import your header file in `integration.c` and write your tests.


### Installation

`criterion` can be installed on debian based distributions using:

```
sudo add-apt-repository ppa:snaipewastaken/ppa
sudo apt-get update
sudo apt-get install criterion-dev
```
