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


### Installation

`criterion` can be installed on debian based distributions using:

```
sudo add-apt-repository ppa:snaipewastaken/ppa
sudo apt-get update
sudo apt-get install criterion-dev
```
