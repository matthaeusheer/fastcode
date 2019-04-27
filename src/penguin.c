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
 * High objective function value = high fitness = high heat radiation = LOW cost as defined in the paper.
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
  return AREA * EPS * SIGMA * pow(308.15, 4.0);
}


/**
   Get the attractiveness between two penguins. Uses the heat radiation and attenuation
   coefficient. Note that the heat radiation determines the direction of attractiveness.
   If `heat_rad` refers to the heat radiation of `penguin_i`, then the attractiveness is
   how much `penguin_j` is attracted to `penguin_i`, not the other way around.
   NOTE: attractiveness actually increases with distance. The reasoning here is that we
   try to minimise the objective value, therefore high heat radiation is bad, and high
   attractiveness is bad as well. We might want to rename all this or change the behaviour to
   make a little more sense.
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
   Compute spiral movement of the penguin towards the centre. See paper on SPO for
   clarification.
 */
double* pen_get_spiral_like_movement(double attract,
                                     size_t dim,
                                     const double *const centre,
                                     const double *const penguin,
                                     const double *const rotation_matrix) {
  // TODO: mem management here is horrible
  double *spiral = (double *) malloc(dim * sizeof(double));
  double* centre_cpy = (double*)malloc(dim * sizeof(double));
  memcpy(centre_cpy, centre, dim * sizeof(double));

  negate(dim, centre_cpy);
  double* tmp = (double*)malloc(dim * sizeof(double));
  vva(dim, penguin, centre_cpy, tmp);
  mvm(dim, rotation_matrix, tmp, spiral);

  free(tmp);
  free(centre_cpy);

  scalar_mul(dim, attract, spiral);

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
  double min = INFINITY;
  size_t min_idx = 0;
  for (size_t idx = 0; idx < colony_size; idx++) {
    if (fitness[idx] < min) {
      min = fitness[idx];
      min_idx = idx;
    }
  }
  return min_idx;
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
   Initialise the rotation matrix of size `dim` times `dim`. Its rotation rate is given by
   `theta` (between -pi and pi).
 */
double* pen_init_rotation_matrix(size_t dim, const double theta) {
  double* matrix = (double*)malloc(dim * dim * sizeof(double));
  double* tmp = (double*)malloc(dim * dim * sizeof(double));
  double* basic_rotation = (double*)malloc(dim * dim * sizeof(double));
  identity(dim, matrix);
  identity(dim, tmp);
  identity(dim, basic_rotation);
  for(size_t idx = 0; idx < dim - 1; idx++) {
    for(size_t runner = idx + 1; runner < dim; runner++) {
      // get basic rotation matrix
      basic_rotation[idx * dim + idx] = cos(theta);
      basic_rotation[idx * dim + runner] = -sin(theta);
      basic_rotation[runner * dim + idx] = sin(theta);
      basic_rotation[runner * dim + runner] = cos(theta);
      negate(dim * dim, basic_rotation);
      mmm(dim, tmp, basic_rotation, matrix);
      memcpy(tmp, matrix, dim * dim * sizeof(double));
      // reset to identity
      negate(dim * dim, basic_rotation);
      basic_rotation[idx * dim + idx] = 1.0;
      basic_rotation[idx * dim + runner] = 0.0;
      basic_rotation[runner * dim + idx] = 0.0;
      basic_rotation[runner * dim + runner] = 1.0;
    }
  }
  free(tmp);
  free(basic_rotation);
  scalar_mul(dim * dim, A, matrix);
  return matrix;
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

  double *population = pen_generate_population(colony_size, dim, min_positions, max_positions);
  double *const fitness = pen_get_initial_fitness(colony_size, dim, population, obj);
  double *const population_cpy = (double*)malloc(colony_size * dim * sizeof(double));
  memcpy(population_cpy, population, colony_size * dim * sizeof(double));

  #ifdef DEBUG
    print_population(colony_size, dim, population); // printing the initial status of the population
    printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));
  #endif

  // initialise rotation matrix
  const double *const r_matrix = pen_init_rotation_matrix(dim, B);


  // initialize coefficients
  double heat_absorption_coef = HAB_COEF_START;
  double mutation_coef = MUT_COEF_START;
  double attenuation_coef = ATT_COEF_START;

  for (size_t iter = 0; iter < max_iterations; iter++) {
    printf("#----- Iteration %lu ----- \n", iter);

    // number of updates for each pengu
    int *const n_updates_per_pengu = filled_int_array(colony_size, 0);

    // actual updates for every pengu, we can select the values by taking all updates up fo n_updates_per_pengu[idx]
    double *const updated_positions = filled_double_array(colony_size * colony_size * dim, 0.0);

    for (size_t penguin_j = 0; penguin_j < colony_size; penguin_j++) {
      for (size_t penguin_i = 0; penguin_i < colony_size; penguin_i++) {
        if (fitness[penguin_j] > fitness[penguin_i]) {  // high fitness = high heat radiation = low cost

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
                                                              &population[penguin_i * dim],
                                                              &population[penguin_j * dim],
                                                              r_matrix);

          // mutate movement
          pen_mutate(dim, spiral, mutation_coef);
          /* negate(dim, spiral); */
          vva(dim, spiral, &population[penguin_j * dim], spiral);


          // clamp
          pen_clamp_position(dim, spiral, min_positions, max_positions);


          print_double_array(dim, spiral);
          memcpy(&updated_positions[penguin_j * dim * colony_size + n_updates_per_pengu[penguin_j] * dim],
                  spiral, dim * sizeof(double));


          //printf("# (i, j) (%lu, %lu): (%f, %f)\n", penguin_i, penguin_j, fitness[penguin_i], fitness[penguin_j]);
          //print_double_array(dim, spiral);
          //printf("\n");

          // fitness[penguin_j] = (*obj)(&population_copy[penguin_j * dim], dim);
          // fitness[penguin_i] = (*obj)(&population[penguin_i * dim], dim);
          free(spiral);
        }
      }
    }


    print_int_array(colony_size, n_updates_per_pengu);

    // accumulate changes for every pengu during this iteration
    for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
      double *const mean_pos = filled_double_array(dim, 0.0);

      if (n_updates_per_pengu[pengu_idx] > 0) {
        for (size_t dim_idx = 0; dim_idx < dim; dim_idx++) {
          double mean_pos_dim = mean_value_in_strides(dim * n_updates_per_pengu[pengu_idx],
                                                      &updated_positions[pengu_idx * dim * colony_size],
                                                      dim_idx,
                                                      dim);
          mean_pos[dim_idx] = mean_pos_dim;
        }
        //printf("# Mean position for pengu %lu\n", pengu_idx);
        //print_double_array(dim, mean_pos);

        // Update positions and fitness
        memcpy(&population[pengu_idx * dim], mean_pos, dim * sizeof(double));
        fitness[pengu_idx] = (*obj)(&population[pengu_idx * dim], dim);

      }
      free(mean_pos);
    }

    free(n_updates_per_pengu);

    // update coefficients
    heat_absorption_coef -= HAB_COEF_STEP;
    mutation_coef -= MUT_COEF_STEP;
    attenuation_coef += ATT_COEF_STEP;

    #ifdef DEBUG
        print_population(colony_size, dim, population);
        printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));
        // size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
        //pen_print_fitness(colony_size, fitness);
        // printf("\nBEST SOLUTION: %ld\n", best_solution);
        //print_solution(dim, &population[best_solution]);
    #endif

  } // loop on iterations

  // final cleanup
  size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
  double *const final_solution = (double *) malloc(dim * sizeof(double));
  memcpy(final_solution, &population[best_solution], dim * sizeof(double));


  #ifdef DEBUG
    //printf("===============================");
    //print_population(colony_size, dim, population);
    //pen_print_fitness(colony_size, fitness);
    //printf("\nBEST SOLUTION: %ld\n", best_solution);
    //  print_solution(dim, &population[best_solution]);
  #endif

  free(population);
  free(fitness);

  return final_solution;
}
