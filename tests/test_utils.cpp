#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "cpp_utils.h"

#include <criterion/criterion.h>

Test(utils_unit, split_str) {
  std::vector<std::string> segments;

  segments = split_str("out_file.txt", '.');
  cr_assert(segments[0] == "out_file");
  cr_assert(segments[1] == "txt");

  segments = split_str("some_random_string", '_');
  cr_assert(segments.size() == 3);
}

Test(utils_unit, add_str_before_file_end) {
  cr_assert(add_str_before_file_end("out_file.txt", "_10") == "out_file_10.txt");
  cr_assert(add_str_before_file_end("some/path/out_file.txt", "_10") == "some/path/out_file_10.txt");
}