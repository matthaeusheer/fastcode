#include <criterion/criterion.h>

#include "../penguin/main.h"

Test(sample_algo, runs) {
  cr_assert(1.0 == algo_returns_1(), "Integration test works");
}
