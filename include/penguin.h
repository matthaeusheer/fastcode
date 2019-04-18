#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"


#define AREA 0.56       // area of penguin surface
#define EPS 0.98        // emissivity of bird plumage
#define SIGMA 5.6703e-8 // stefan-boltzmann constant

// attenuation coefficient
// should be above 1.0 at all times in order to not revert the exponent in equation 11
#define ATT_COEF_START 1.0
#define ATT_COEF_STEP 0.001

// heat absorption coefficient
#define HAB_COEF_START 1
#define HAB_COEF_STEP 0.001

// mutation coefficient
#define MUT_COEF_START 0.2
#define MUT_COEF_STEP 0.001

#define A 3       // see equ 18 from paper
#define B 0.1     // see equ 18 from paper


// helper utilities
#define pen_max(a, b)                           \
  ({__typeof__ (a) _a = (a);                    \
    __typeof__ (b) _b = (b);                    \
    _a > _b ? _a : _b;})

#define pen_min(a, b)                           \
  ({__typeof__ (a) _a = (a);                    \
    __typeof__ (b) _b = (b);                    \
    _a < _b ? _a : _b;})


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
                            const double * min_positions,
                            const double * max_positions);

/**
   Generate a full random penguin population of size `colony_size`,
   where each penguin has `dim` dimensions. The minimal and maximal
   values these dimensions can take are bound by `min_positions` and
   `max_positions`.
 */
double *pen_generate_population(size_t colony_size,
                                size_t dim,
                                const double * min_positions,
                                const double * max_positions);

/**
   Generates a copy of the population array.
 */
double *pen_copy_population(size_t colony_size, size_t dim,
                            const double * population);

/**
   Get the initial fitness values for each penguin in the population. This will
   generate a new fitness array.
 */
double *pen_get_initial_fitness(size_t colony_size,
                                size_t dim,
                                const double * population,
                                double(*obj)(const double *const, size_t));

/**
   Compute the euclidean distance between two penguins.
 */
double pen_eucledian_distance(size_t dim,
                              const double * penguin_i,
                              const double * penguin_j);

/**
   Computes the heat radiation of a penguin. See equation 6 in paper.
 */
double pen_heat_radiation(double fitness);

/**
   Get the attractiveness between two penguins. Uses the heat radiation and attenuation
   coefficient. Note that the heat radiation determines the direction of attractiveness.
   If `heat_rad` refers to the heat radiation of `penguin_i`, then the attractiveness is
   how much `penguin_j` is attracted to `penguin_i`, not the other way around.
 */
double pen_attractiveness(double heat_rad,
                          size_t dim,
                          const double * penguin_i,
                          const double * penguin_j,
                          double attenuation_coef);

/**
   Computes the common sum in equation 18 from the paper.
 */
double pen_compute_common_sum(double attract, double x_i, double y_i, double x_j, double y_j);

/**
   Computes the factor in the position update equation 18 from the paper.
 */
double pen_compute_factor(double attract, double x_i, double y_i, double x_j, double y_j);

/**
   Computes the x_k update value from equation 18 from the paper.
 */
double pen_compute_x_k(double attract, double x_i, double y_i, double x_j, double y_j);

/**
   Computes the y_k update value from equation 18 from the paper.
 */
double pen_compute_y_k(double attract, double x_i, double y_i, double x_j, double y_j);

/**
   Compute spiral movement of the penguin towards the centre. See paper on SPO for
   clarification.
 */
double *pen_get_spiral_like_movement(double attract, size_t dim,
                                     const double * centre, const double * penguin,
                                     const double* rotation_matrix);


/**
   Initialise the rotation matrix of size `dim` times `dim`. Its rotation rate is given by
   `theta` (between -pi and pi).
 */
double* pen_init_rotation_matrix(size_t dim, const double theta);

/**
   Mutates the spiral according the equation 19 from the paper. This modifies the spiral
   in place.
 */
void pen_mutate(size_t dim, double * spiral, double mutation_coef);

/**
   Clamps the solution in the possible range. This is done in place.
 */
void pen_clamp_position(size_t dim, double * position,
                        const double * min_positions,
                        const double * max_positions);

/**
   Gets the index of the fittest penguin in the population. Note that the less fit, the better.
 */
size_t pen_get_fittest_idx(size_t colony_size, const double *  fitness);


/**
   Prints the fitness to standard output.
*/
void pen_print_fitness(size_t colony_size, double * fitness);

#ifdef __cplusplus
}
#endif // __cplusplus
