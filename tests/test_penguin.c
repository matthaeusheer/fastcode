// This is a sample algo to show how the test suite works. Nothing more.

#include <criterion/criterion.h>

#include "penguin.h"

// unittest
Test(unittest, runs) {
    cr_assert(1.0 == algo_returns_1(), "Unit test works");
}
