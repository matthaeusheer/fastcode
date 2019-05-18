/**
   Base implementation of the emperor penguin metaheuristic.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "penguin.h"


/**
 * Returns a pointer to a float array which is of size colony_size * dim.
 * The stride over penguins is dim since every penguin (representing possible solution) is of size dim.
 */
void pen_initialise_population(float* const population,
                                size_t colony_size,
                                size_t dim,
                                const float min_position,
                                const float max_position) {
  for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
    for (size_t dim_idx = 0; dim_idx < dim; dim_idx++) {
      size_t idx = (pengu_idx * dim) + dim_idx;
      population[idx] = random_min_max(min_position, max_position);
    }
  }
}

/**
 * Calculates initial fitness of each penguin by plugging its dim dimensions
 * into the objective function. The returned fitness array is of size colony_size.
 * High objective function value = high fitness = high heat radiation = LOW cost as defined in the paper.
 */
void pen_update_fitness(float* const fitness,
                        size_t colony_size,
                        size_t dim,
                        const float *const population,
                        obj_func_t obj_func) {
  for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
    size_t idx = pengu_idx * dim;
    fitness[pengu_idx] = (*obj_func)(&population[idx], dim);
  }
}


/**
   Compute the euclidean distance between two penguins.
   The dim-dimensional penguins are subtracted and the absolute value is taken.
 */
float pen_eucledian_distance(size_t dim,
                              const float *const penguin_i,
                              const float *const penguin_j) {
  float dist = 0.0;
  for (size_t idx = 0; idx < dim; idx++) {
    dist += pow(penguin_i[idx] - penguin_j[idx], 2.0);
  }
  return sqrt(dist);
}


/**
   Computes the heat radiation of a penguin. See equation 6 in paper.
 */
float pen_heat_radiation() {
  return AREA * EPS * SIGMA * pow(BODY_TEMP, 4.0);
}


/**
   Obtain the attractiveness between a pair of penguins. Note that since the
   heat radiation is no longer based on fitness, this value is purely based on
   the distance between the penguins and therefore their order in the arguments
   no longer matters.
 */
float pen_attractiveness(float heat_rad,
                          size_t dim,
                          const float *const penguin_i,
                          const float *const penguin_j,
                          float attenuation_coef) {
  float dist = pen_eucledian_distance(dim, penguin_i, penguin_j);
  return heat_rad * exp(-dist * attenuation_coef);
}


/**
   Compute spiral movement of the penguin towards the centre. See paper on SPO for
   clarification.
 */
void pen_get_spiral_like_movement(float* const spiral,
                                  float attract,
                                  size_t dim,
                                  const float *const centre,
                                  const float *const penguin,
                                  const float *const rotation_matrix) {
  // float* centre_cpy = (float*)malloc(dim*sizeof(float));
  float centre_cpy[dim];
  memcpy(centre_cpy, centre, dim * sizeof(float));

  negate(dim, centre_cpy);
  // float* tmp = (float*)malloc(dim*sizeof(float));
  float tmp[dim];
  vva(dim, penguin, centre_cpy, tmp);
  mvm(dim, rotation_matrix, tmp, spiral);

  scalar_mul(dim, attract, spiral);

  // free(tmp);
  // free(centre_cpy);
}


/**
   Mutates the spiral according the equation 19 from the paper.
   Caution: This modifies the spiral in place!
 */
void pen_mutate(size_t dim, float *const spiral, float mutation_coef) {
  for (size_t idx = 0; idx < dim; idx++) {
    spiral[idx] += random_min_max(-1.0, 1.0) * mutation_coef;
  }
}


/**
   Clamps the solution in the possible range. This is done in place.
 */
void pen_clamp_position(size_t dim, float *const position,
                        const float min_position,
                        const float max_position) {
  for (size_t idx = 0; idx < dim; idx++) {
    position[idx] = pen_max(position[idx], min_position);
    position[idx] = pen_min(position[idx], max_position);
  }
}


/**
   Gets the index of the fittest penguin in the population.
   The fitness (obj function value) goes into the attraction calculation. The higher the fitness the higher
   the attraction the higher the convergence. So getting the fittest idx means getting the one where the
   objective function value (fitness) is highest.
 */
size_t pen_get_fittest_idx(size_t colony_size, const float *const fitness) {
  float min = INFINITY;
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
void pen_init_rotation_matrix(float* const matrix, size_t dim, const float theta) {

  float* tmp = (float*)malloc(dim*dim*sizeof(float));
  float* basic_rotation = (float*)malloc(dim*dim*sizeof(float));

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
      memcpy(tmp, matrix, dim * dim * sizeof(float));
      // reset to identity
      negate(dim * dim, basic_rotation);
      basic_rotation[idx * dim + idx] = 1.0;
      basic_rotation[idx * dim + runner] = 0.0;
      basic_rotation[runner * dim + idx] = 0.0;
      basic_rotation[runner * dim + runner] = 1.0;
    }
  }
  scalar_mul(dim * dim, A, matrix);

  free(tmp);
  free(basic_rotation);
}

/**
   Run the emperor penguin metaheuristic.
   Arguments:
     - obj_func: the objective function accepting an array of floats and the dimension.
     - dim: the dimension count of a solution (penguin).
     - max_iterations: the maximal count of iterations to be run.
     - min_position: the minimum value for each dimension
     - max_position: the maximum value for each dimension.
   Returns:
     An array of floats representing a solution. The length of the array is
     `dim`.
 */
float *pen_emperor_penguin(obj_func_t obj_func,
                            size_t colony_size,
                            size_t dim,
                            size_t max_iterations,
                            const float min_position,
                            const float max_position) {
  srand(100);

  // initialise data
  float* population = (float*)malloc(colony_size*dim*sizeof(float));
  pen_initialise_population(population, colony_size, dim, min_position, max_position);

  // float fitness[colony_size];
  float* fitness = (float*)malloc(colony_size*sizeof(float));
  pen_update_fitness(fitness, colony_size, dim, population, obj_func);

  float* r_matrix = (float*)malloc(dim*dim*sizeof(float));
  pen_init_rotation_matrix(r_matrix, dim, B);

  float base_heat_radiation = pen_heat_radiation();

  #ifdef DEBUG
    print_population(colony_size, dim, population); // printing the initial status of the population
    printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));
    printf("# BEST FITNESS: %f\n", lowest_value(colony_size, fitness));
  #endif

  // initialise rotation matrix

  for (size_t iter = 0; iter < max_iterations; iter++) {

    // initialize coefficients
    float heat_absorption_coef = linear_scale(HAB_COEF_START, HAB_COEF_END, max_iterations, iter);
    float mutation_coef = linear_scale(MUT_COEF_START, MUT_COEF_END, max_iterations, iter);
    float attenuation_coef = linear_scale(ATT_COEF_START, ATT_COEF_END, max_iterations, iter);

    // number of updates for each pengu
    int* n_updates_per_pengu = (int*)malloc(colony_size*sizeof(int));
    fill_int_array(n_updates_per_pengu, colony_size, 0);

    // float updated_positions[colony_size * colony_size * dim];
    float* updated_positions = (float*)malloc(colony_size*colony_size*dim*sizeof(float));
    fill_float_array(updated_positions, colony_size * colony_size * dim, 0.0);


    for (size_t penguin_j = 0; penguin_j < colony_size; penguin_j++) {
      for (size_t penguin_i = 0; penguin_i < colony_size; penguin_i++) {
        if (fitness[penguin_j] > fitness[penguin_i]) {   // high fitness = high heat radiation = low cost // high fitness = high heat radiation = low cost

          // calculate heat radiation
          float heat_rad = heat_absorption_coef * base_heat_radiation;

          // calculate attractiveness
          float attract = pen_attractiveness(heat_rad,
                                              dim,
                                              &population[penguin_i * dim],
                                              &population[penguin_j * dim],
                                              attenuation_coef);

          // calculate spiral movement
          // float* spiral = (float*)malloc(dim*sizeof(float));
          float spiral[dim];
          pen_get_spiral_like_movement(spiral, attract, dim,
                                       &population[penguin_i * dim],
                                       &population[penguin_j * dim],
                                       r_matrix);

          // mutate movement
          pen_mutate(dim, spiral, mutation_coef);

          // update position by adding spiral movement on top of old position
          vva(dim, spiral, &population[penguin_j * dim], spiral);

          // clamp
          pen_clamp_position(dim, spiral, min_position, max_position);

          // add movement to updated position
          memcpy(&updated_positions[penguin_j * dim * colony_size + n_updates_per_pengu[penguin_j] * dim],
                  spiral, dim * sizeof(float));
          n_updates_per_pengu[penguin_j] += 1;

          // free(spiral);
        }
      }
    }

    // accumulate changes for every pengu during this iteration
    for (size_t pengu_idx = 0; pengu_idx < colony_size; pengu_idx++) {
      // float* mean_pos = (float*)malloc(dim*sizeof(float));
      float mean_pos[dim];
      fill_float_array(mean_pos, dim, 0.0);

      if (n_updates_per_pengu[pengu_idx] > 0) {
        for (size_t dim_idx = 0; dim_idx < dim; dim_idx++) {
          float mean_pos_dim = mean_value_in_strides(dim * n_updates_per_pengu[pengu_idx],
                                                      &updated_positions[pengu_idx * dim * colony_size],
                                                      dim_idx,
                                                      dim);
          mean_pos[dim_idx] = mean_pos_dim;
        }
        // finally positions and fitness for a whole iteration
        memcpy(&population[pengu_idx * dim], mean_pos, dim * sizeof(float));
        fitness[pengu_idx] = (*obj_func)(&population[pengu_idx * dim], dim);
      }
      // free(mean_pos);
    }

    free(n_updates_per_pengu);
    free(updated_positions);

    #ifdef DEBUG
      print_population(colony_size, dim, population);
      printf("# AVG FITNESS: %f\n", average_value(colony_size, fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(colony_size, fitness));
    #endif
  } // end loop on iterations

  // final selection and cleanup
  size_t best_solution = pen_get_fittest_idx(colony_size, fitness);
  float *const final_solution = (float *) malloc(dim * sizeof(float));
  memcpy(final_solution, &population[best_solution], dim * sizeof(float));

  free(r_matrix);
  free(population);
  free(fitness);

  return final_solution;
}
