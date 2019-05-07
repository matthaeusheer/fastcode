#include <iostream>

#include <fstream>
#include <algorithm>
#include <getopt.h>

#include "cpp_utils.h"

#define ARGC_REQUIRED 20

#define USAGE (                                                         \
               "\nUsage:  [-vaofsnmpyz]\n"                              \
               "  -v    verbose\n"                                      \
               "  -a    algorithm name\n"                               \
               "  -o    objective function name\n"                      \
               "  -f    output timing file name\n"                      \
               "  -s    output solution file name\n"                    \
               "  -n    number of iterations to run per algorithm\n"    \
               "  -m    number of repetitions of an algorihtm\n"        \
               "  -p    population size\n"                              \
               "  -y    minimum values\n"                               \
               "  -z    maximum values\n"                               \
               "  \n"                                                   \
               "Currently all parameters are required.\n\n"             \
               "Example:\n"                                             \
               "    ./benchmark -a \"hgwosca\" -o \"sum\" -n 50 -m 1 -d 10 -p 30 -y -120 -z 100 -s \"../data/solution.txt\" -f \"../data/timings.txt\"\n")


/**
   Parse command line arguments.
*/
void parse_args(Config *config, int argc, char *argv[]) {

  if (argc < ARGC_REQUIRED) {
    std::cout << USAGE << std::endl;
    exit(1);
  }

  int opt;

  // Default values for optional arguments.
  config->verbose = false;
  config->algorithm = "";
  config->solution_file = "";
  config->out_file = "";

  while ((opt = getopt(argc, argv, "hva:o:d:p:n:m:y:z:f:s:")) != -1) {
    switch (opt) {
      case 'v':  // verbose
        config->verbose = true;
        break;
      case 'a':  // algorithm
        config->algorithm = std::string(optarg);
        break;
      case 'o':  // objective function
        config->obj_func = std::string(optarg);
        break;
      case 'f':  // output file name
        config->out_file = std::string(optarg);
        break;
      case 's':  // solution output name
        config->solution_file = std::string(optarg);
        break;
      case 'd':  //  dimension
        int dim;
        if (sscanf(optarg, "%i", &dim) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", opt);
          exit(EXIT_FAILURE);
        }
        config->dimension = dim;
        break;
      case 'p':  // population
        int population;
        if (sscanf(optarg, "%i", &population) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        config->population = population;
        break;
      case 'n':  // n_iterations
        int n_iter;
        if (sscanf(optarg, "%i", &n_iter) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        config->n_iterations = n_iter;
        break;
      case 'm':  // n_repetitions
        int n_repetitions;
        if (sscanf(optarg, "%i", &n_repetitions) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        config->n_repetitions = n_repetitions;
        break;
      case 'y':  // min_position
        int min_pos;
        if (sscanf(optarg, "%i", &min_pos) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        config->min_position = min_pos;
        break;
      case 'z':  // max_position
        int max_pos;
        if (sscanf(optarg, "%i", &max_pos) != 1) {
          fprintf(stderr, "invalid arg '%s': must be an integer\n", optarg);
          exit(EXIT_FAILURE);
        }
        config->max_position = max_pos;
        break;

      case 'h':
      default:
        fprintf(stderr, USAGE);
        exit(EXIT_FAILURE);
    }
  }
}


void print_file(std::string file_path) {

  std::string text;
  std::ifstream ifs(file_path);

  while (!ifs.eof()) {
    std::getline(ifs, text);
    std::cout << "" << text << "\n";
  }
}

// TODO: This feature is not ported to CMake build system yet.
void print_git_version() {
  std::cout << "# Git" << std::endl;
  //std::cout << "git version - " << GIT_VERSION << std::endl;
  //std::cout << "git commit - " << GIT_COMMIT << std::endl;
  //std::cout << "git date - " << GIT_DATE << std::endl;
}


void print_config(Config config) {
  std::cout << "\n=========================================== " << std::endl;
  std::cout << "  Algorithm:          " << config.algorithm     << std::endl;
  std::cout << "  Objective function: " << config.obj_func      << std::endl;
  std::cout << "  Dimension:          " << config.dimension     << std::endl;
  std::cout << "  Population          " << config.population    << std::endl;
  std::cout << "  N Iterations:       " << config.n_iterations  << std::endl;
  std::cout << "  Min position:       " << config.min_position  << std::endl;
  std::cout << "  Max position        " << config.max_position  << std::endl;
  std::cout << " ===========================================\n" << std::endl;
}


void store_timings(const std::vector<unsigned long long> &cycles_vec, std::string file_path) {

  if (file_path != "") {
    std::ofstream outfile;
    outfile.open(file_path);

    outfile << "iteration,cycles" << std::endl;
    for (int idx = 0; idx < cycles_vec.size(); ++idx) {
      outfile << idx << ", " << cycles_vec[idx] << std::endl;
    }

    outfile.close();

    std::cout << "Stored timings in: " << file_path << std::endl;
  } else {
    std::cout << "Output file parameter is default. No storage of timings." << std::endl;
  }
}

void store_solutions(float *solution, int dimension, std::string file_path) {

  if (file_path != "") {
    std::ofstream outfile;
    outfile.open(file_path);

    outfile << "dimension,value" << std::endl;

    for (size_t idx = 0; idx < dimension; ++idx) {
      outfile << idx << ", " << solution[idx] << std::endl;
    }

    outfile.close();

    std::cout << "Stored solution in: " << file_path << std::endl;
  } else {
    std::cout << "Solution output file parameter is default. No storage of timings." << std::endl;
  }
}


std::vector<std::string> split_str(const std::string &s, char c) {
  std::vector<std::string> v;
  std::string::size_type i = 0;
  std::string::size_type j = s.find(c);

  while (j != std::string::npos) {
    v.push_back(s.substr(i, j - i));
    i = ++j;
    j = s.find(c, j);

    if (j == std::string::npos)
      v.push_back(s.substr(i, s.length()));
  }
  return v;
}

std::string add_str_before_file_end(std::string file_path, std::string insert) {
  std::size_t pos_last_slash = file_path.rfind('/');
  std::size_t pos_last_dot = file_path.rfind('.');
  auto file_ending = file_path.substr(pos_last_dot + 1, std::string::npos);

  if (std::string::npos == pos_last_slash) {  // e.g. outfile.txt
    return split_str(file_path, '.').front() + insert + "." + file_ending;
  }

  // else: e.g. /some/path/outfile.txt or ../relative/path/outfile.txt

  auto path_without_file_name = file_path.substr(0, pos_last_slash);
  auto file_name = split_str(file_path, '/').back();
  long dot_count = std::count(file_name.begin(), file_name.end(), '.');
  if (dot_count != 1) {
    throw std::invalid_argument("Input string (file_name) should contain exactly one dot. "
                                "E.g. out.file.txt not allowed.");
  }

  auto new_path = path_without_file_name + "/" + split_str(file_name, '.').front() + insert + "." + file_ending;

  return new_path;
}
