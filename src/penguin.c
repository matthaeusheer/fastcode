/**
   Base implementation of the emperor penguin metaheuristic.
   NOTE: This metaheuristic maximises fitness, as opposed to minimising it
   like many other metaheuristics do. Consider this when feeding it the
   objective function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "penguin.h"


/**
 * Returns a pointer to a double array which is of size colony_size * dim.
 * The stride over penguins is dim since every penguin (representing possible solution) is of size dim.
 */
double *pen_generate_population(size_t colony_size,
                                size_t dim,
                                const double *const min_positions,
                                const double *const max_positions) {
  double *population = (double *) malloc(colony_size * dim * sizeof(double));
  for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
    for (size_t dim_idx = 0; dim_idx < dim; dim_idx++) {
      size_t idx = (pengu_idx * dim) + dim_idx;
      population[idx] = random_min_max(min_positions[dim_idx], max_positions[dim_idx]);
    }
  }
  return population;
}

/**
 * Deep copy of a penguin population.
 */
double *pen_copy_population(size_t colony_size, size_t dim,
                            const double *const population) {
  size_t size = colony_size * dim * sizeof(double);
  double *copy = (double *) malloc(size);
  memcpy(copy, population, size);
  return copy;
}

/**
 * Calculates initial fitness of each penguin by plugging its dim dimensions
 * into the objective function. The returned fitness array is of size colony_size.
 */
double *pen_get_initial_fitness(size_t colony_size,
                                size_t dim,
                                const double *const population,
                                double(*obj)(const double *const, size_t)) {
  double *fitness = (double *) malloc(colony_size * sizeof(double));
  for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
    size_t idx = pengu_idx * dim;
    fitness[pengu_idx] = (*obj)(&population[idx], dim);
  }
  return fitness;
}


/**
   Compute the euclidean distance between two penguins.
   The dim-dimensional penguins are subtracted and the absolute value is taken.
 */
double pen_eucledian_distance(size_t dim,
                              const double *const penguin_i,
                              const double *const penguin_j) {
  double dist = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    dist += pow(penguin_i[idx] - penguin_j[idx], 2.0);
  }
  return sqrt(dist);
}


/**
   Computes the heat radiation of a penguin. See equation 6 in paper.
 */
double pen_heat_radiation(double fitness) {
  return AREA * EPS * SIGMA * pow(fitness, 4.0);
}


/**
   Get the attractiveness between two penguins. Uses the heat radiation and attenuation
   coefficient. Note that the heat radiation determines the direction of attractiveness.
   If `heat_rad` refers to the heat radiation of `penguin_i`, then the attractiveness is
   how much `penguin_j` is attracted to `penguin_i`, not the other way around.
 */
double pen_attractiveness(double heat_rad,
                          size_t dim,
                          const double *const penguin_i,
                          const double *const penguin_j,
                          double attenuation_coef) {
  double dist = pen_eucledian_distance(dim, penguin_i, penguin_j);
  return heat_rad * exp(-dist * attenuation_coef);
}


/**
   Computes the common sum in equation 18 from the paper.
 */
double pen_compute_common_sum(double attract,
                              double x_i, double y_i,
                              double x_j, double y_j) {
  double exponent_term1 = (1 - attract) * exp(B * atan(y_i / x_i));
  double exponent_term2 = attract * exp(B * atan(y_j / x_j));
  return 1 / B * log(exponent_term1 + exponent_term2);
}


/**
   Computes the factor in the position update equation 18 from the paper.
 */
double pen_compute_factor(double attract,
                          double x_i, double y_i,
                          double x_j, double y_j) {
  double exponent = pen_compute_common_sum(attract, x_i, y_i, x_j, y_j);
  return A * exp(B * exponent);
}


/**
   Computes the x_k update value from equation 18 from the paper.
 */
double pen_compute_x_k(double attract,
                       double x_i, double y_i,
                       double x_j, double y_j) {
  double factor = pen_compute_factor(attract, x_i, y_i, x_j, y_j);
  double cos_arg = pen_compute_common_sum(attract, x_i, y_i, x_j, y_j);
  return factor * cos(cos_arg);
}


/**
   Computes the y_k update value from equation 18 from the paper.
 */
double pen_compute_y_k(double attract,
                       double x_i, double y_i,
                       double x_j, double y_j) {
  double factor = pen_compute_factor(attract, x_i, y_i, x_j, y_j);
  double cos_arg = pen_compute_common_sum(attract, x_i, y_i, x_j, y_j);
  return factor * sin(cos_arg);
}


/**
   Compute spiral like movement. See equation 18 in paper.
   TODO: this function currently only supports computing the spiral movement for two
   dimensional penguins. This is not usable in practice, since most objective functions
   will have more than two parameters. This needs to be generalized.
 */
double *pen_get_spiral_like_movement(double attract,
                                     size_t dim,
                                     const double *const penguin_i,
                                     const double *const penguin_j) {
  double *spiral = (double *) malloc(dim * sizeof(double));

  for (size_t dimension = 0; dimension < dim; dimension += 2) {
    double x_i = penguin_i[dimension];
    double y_i = penguin_i[dimension + 1];
    double x_j = penguin_j[dimension];
    double y_j = penguin_j[dimension + 1];
    spiral[dimension] = pen_compute_x_k(attract, x_i, y_i, x_j, y_j);
    spiral[dimension + 1] = pen_compute_y_k(attract, x_i, y_i, x_j, y_j);
  }

  return spiral;
}


/**
   Mutates the spiral according the equation 19 from the paper.
   Caution: This modifies the spiral in place!
 */
void pen_mutate(size_t dim, double *const spiral, double mutation_coef) {
  for (size_t idx = 0; idx < dim; idx++) {
    spiral[idx] += random_min_max(-1.0, 1.0) * mutation_coef;
  }
}


/**
   Clamps the solution in the possible range. This is done in place.
 */
void pen_clamp_position(size_t dim, double *const position,
                        const double *const min_positions,
                        const double *const max_positions) {
  for (size_t idx = 0; idx < dim; idx++) {
    position[idx] = pen_max(position[idx], min_positions[idx]);
    position[idx] = pen_min(position[idx], max_positions[idx]);
  }
}


/**
   Gets the index of the fittest penguin in the population.
   The fitness (obj function value) goes into the attraction calculation. The higher the fitness the higher
   the attraction the higher the convergence. So getting the fittest idx means getting the one where the
   objective function value (fitness) is highest.
 */
size_t pen_get_fittest_idx(size_t colony_size, const double *const fitness) {
  double max = -INFINITY;
  size_t max_idx = 0;
  for (size_t idx = 0; idx < colony_size; idx++) {
    if (fitness[idx] > max) {
      max = fitness[idx];
      max_idx = idx;
    }
  }
  return max_idx;
}


/**
   Prints the population to standard output.
 */
void pen_print_pop(size_t colony_size, size_t dim, const double *const population) {
  for (size_t idx = 0; idx < colony_size; idx++) {
    printf("pengu%03ld, ", idx);
    print_solution(dim, &population[idx * dim]);
  }
}


/**
   Prints the fitness to standard output.
*/
void pen_print_fitness(size_t colony_size, double *const fitness) {
  for (size_t idx = 0; idx < colony_size; idx++) {
    printf("Pengion %03ld:\t%8.4f\n", idx, fitness[idx]);
  }
}


/**
   Run the emperor penguin metaheuristic.
   Arguments:
     - obj: the objective function accepting an array of doubles and the dimension.
     - dim: the dimension count of a solution (penguin).
     - max_iterations: the maximal count of iterations to be run.
     - min_positions: an array of the minimum values for each dimension.
     - max_positions: an array of the maximum values for each dimension.
   Returns:
     An array of doubles representing a solution. The length of the array is
     `dim`.
 */
double *pen_emperor_penguin(double(*obj)(const double *const, size_t),
                            size_t colony_size,
                            size_t dim,
                            size_t max_iterations,
                            const double *const min_positions,
                            const double *const max_positions) {
  // seed rng
  srand((unsigned) time(NULL));

  double *const population = pen_generate_population(colony_size, dim, min_positions, max_positions);
  double *const fitness = pen_get_initial_fitness(colony_size, dim, population, obj);

  #ifdef DEBUG
    pen_print_pop(colony_size, dim, population); // printing the initial status of the population
  #endif

  // initialize coefficients
  double heat_absorption_coef = HAB_COEF_START;
  double mutation_coef = MUT_COEF_START;
  double attenuation_coef = ATT_COEF_START;

  for (size_t iter = 0; iter < max_iterations; iter++) {

    for (size_t penguin_i = 0; penguin_i < colony_size; penguin_i++) {
      for (size_t penguin_j = 0; penguin_j < colony_size; penguin_j++) {
        if (fitness[penguin_j] < fitness[penguin_i]) {

          // calculate heat radiation
          double heat_rad = heat_absorption_coef * pen_heat_radiation(fitness[penguin_i]);

          // calculate attractiveness
          double attract = pen_attractiveness(heat_rad,
                                              dim,
                                              &population[penguin_i * dim],
                                              &population[penguin_j * dim],
                                              attenuation_coef);

          // calculate spiral movement
          double *const spiral = pen_get_spiral_like_movement(attract, dim,
                                                              &population[penguin_j * dim],
                                                              &population[penguin_i * dim]);

          // mutate movement
          pen_mutate(dim, spiral, mutation_coef);

          // clamp
          pen_clamp_position(dim, spiral, min_positions, max_positions);
          /* TODO: check if this should really be updated in the copy of the array. Seems to me
          it can extremely be overwritten at a later point in time by another pair of penguins
          containing the same penguin_j. */
          memcpy(&population[penguin_j * dim], spiral, dim * sizeof(double));
          fitness[penguin_j] = (*obj)(&population[penguin_j * dim], dim);
          fitness[penguin_i] = (*obj)(&population[penguin_i * dim], dim);

          free(spiral);
        }
      }
    }


    #ifdef DEBUG
      //printf("=====\n");
      size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
      pen_print_pop(colony_size, dim, population);
      //pen_print_fitness(colony_size, fitness);
      //printf("\nBEST SOLUTION: %ld\n", best_solution);
      //print_solution(dim, &population[best_solution]);
    #endif


    heat_absorption_coef -= HAB_COEF_STEP;
    mutation_coef -= MUT_COEF_STEP;
    attenuation_coef += ATT_COEF_STEP;
  }

  size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
  double *const final_solution = (double *) malloc(dim * sizeof(double));
  memcpy(final_solution, &population[best_solution], dim * sizeof(double));

  #ifdef DEBUG
    //printf("===============================");
    //pen_print_pop(colony_size, dim, population);
    //pen_print_fitness(colony_size, fitness);
    //printf("\nBEST SOLUTION: %ld\n", best_solution);
    //  print_solution(dim, &population[best_solution]);
  #endif

  free(population);
  free(fitness);

  return final_solution;
}