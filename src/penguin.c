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
 * Calculates initial fitness of each penguin by plugging its dim dimensions
 * into the objective function. The returned fitness array is of size colony_size.
 * High objective function value = high fitness = high heat radiation = LOW cost as defined in the paper.
 */
double *pen_get_initial_fitness(size_t colony_size,
                                size_t dim,
                                const double *const population,
                                obj_func_t obj_func) {
  double *fitness = (double *) malloc(colony_size * sizeof(double));
  for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
    size_t idx = pengu_idx * dim;
    fitness[pengu_idx] = (*obj_func)(&population[idx], dim);
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
double pen_heat_radiation() {
  return AREA * EPS * SIGMA * pow(308.15, 4.0);
}


/**
   Obtain the attractiveness between a pair of penguins. Note that since the
   heat radiation is no longer based on fitness, this value is purely based on
   the distance between the penguins and therefore their order in the arguments
   no longer matters.
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
   Compute spiral movement of the penguin towards the centre. See paper on SPO for
   clarification.
 */
double* pen_get_spiral_like_movement(double attract,
                                     size_t dim,
                                     const double *const centre,
                                     const double *const penguin,
                                     const double *const rotation_matrix) {
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
     - obj_func: the objective function accepting an array of doubles and the dimension.
     - dim: the dimension count of a solution (penguin).
     - max_iterations: the maximal count of iterations to be run.
     - min_positions: an array of the minimum values for each dimension.
     - max_positions: an array of the maximum values for each dimension.
   Returns:
     An array of doubles representing a solution. The length of the array is
     `dim`.
 */
double *pen_emperor_penguin(obj_func_t obj_func,
                            size_t colony_size,
                            size_t dim,
                            size_t max_iterations,
                            const double *const min_positions,
                            const double *const max_positions) {
  srand((unsigned) time(NULL));

  double *population = pen_generate_population(colony_size, dim, min_positions, max_positions);
  double *const fitness = pen_get_initial_fitness(colony_size, dim, population, obj_func);

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
    // number of updates for each pengu
    int *const n_updates_per_pengu = filled_int_array(colony_size, 0);

    // actual updates for every pengu, we can select the values by taking all updates up fo n_updates_per_pengu[idx]
    double *const updated_positions = filled_double_array(colony_size * colony_size * dim, 0.0);

    for (size_t penguin_j = 0; penguin_j < colony_size; penguin_j++) {
      for (size_t penguin_i = 0; penguin_i < colony_size; penguin_i++) {
        if (fitness[penguin_j] > fitness[penguin_i]) {  // high fitness = high heat radiation = low cost

          // calculate heat radiation
          double heat_rad = heat_absorption_coef * pen_heat_radiation();

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

          // update position by adding spiral movement on top of old position
          vva(dim, spiral, &population[penguin_j * dim], spiral);

          // clamp
          pen_clamp_position(dim, spiral, min_positions, max_positions);


          // add movement to updated position
          memcpy(&updated_positions[penguin_j * dim * colony_size + n_updates_per_pengu[penguin_j] * dim],
                  spiral, dim * sizeof(double));
          n_updates_per_pengu[penguin_j] += 1;

          // printf("# (i, j) (%lu, %lu): (%f, %f)\n", penguin_i, penguin_j, fitness[penguin_i], fitness[penguin_j]);

          free(spiral);
        }
      }
    }

    // TODO: Factor out this part to own function.
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

        // finally positions and fitness for a whole iteration
        memcpy(&population[pengu_idx * dim], mean_pos, dim * sizeof(double));
        fitness[pengu_idx] = (*obj_func)(&population[pengu_idx * dim], dim);

      }
      free(mean_pos);
    }

    free(n_updates_per_pengu);
    free(updated_positions);

    // update coefficients
    heat_absorption_coef -= HAB_COEF_STEP;
    mutation_coef -= MUT_COEF_STEP;
    attenuation_coef += ATT_COEF_STEP;

    #ifdef DEBUG
        print_population(colony_size, dim, population);
        printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));
    #endif

  } // end loop on iterations

  // final selection and cleanup
  size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
  double *const final_solution = (double *) malloc(dim * sizeof(double));
  memcpy(final_solution, &population[best_solution], dim * sizeof(double));

  free(population);
  free(fitness);

  return final_solution;
}
