// This is a sample algo to show how the test suite works. Nothing more.

#include <criterion/criterion.h>

#include "main.h"

double algo_returns_1() {
  return 1.0;
}


// unittest
Test(unittest, runs) {
  cr_assert(1.0 == algo_returns_1(), "Unit test works");
}
