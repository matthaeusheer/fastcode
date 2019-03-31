#pragma once

#include <string>
#include <utility>

#include "../objectives/objectives.h"


typedef std::function<void()> algo_func_t;
typedef std::map<std::string, std::string> config_t;

std::pair<std::vector<int>, std::vector<int>> time_algorithm(config_t config);
algo_func_t get_algo_func(config_t cfg, size_t input_size, obj_map_t& obj_map);


