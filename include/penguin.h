#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"


#define AREA 0.56       // area of penguin surface
#define EPS 0.98        // emissivity of bird plumage
#define SIGMA 5.6703e-8 // stefan-boltzmann constant

#define BODY_TEMP 308.15 // 35 degrees Celcius in Kelvin

// attenuation coefficient
// should be above 1.0 at all times in order to not revert the exponent in equation 11
#define ATT_COEF_START 0.0
#define ATT_COEF_END 1.0

// heat absorption coefficient
#define HAB_COEF_START 1
#define HAB_COEF_END 0

// mutation coefficient
#define MUT_COEF_START 0.1
#define MUT_COEF_END 0

#define A 0.1       // see equ 18 from paper
#define B 0.5       // see equ 18 from paper



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
     - obj_func: the objective function accepting an array of floats and the dimension.
     - dim: the dimension count of a solution (penguin).
     - max_iterations: the maximal count of iterations to be run.
     - min_positions: an array of the minimum values for each dimension.
     - max_positions: an array of the maximum values for each dimension.
   Returns:
     An array of floats representing a solution. The length of the array is
     `dim`.
 */
float *pen_emperor_penguin(obj_func_t obj_func,
                            size_t colony_size,
                            size_t dim,
                            size_t max_iterations,
                            const float min_position,
                            const float max_position);

/**
   Generate a full random penguin population of size `colony_size`,
   where each penguin has `dim` dimensions. The minimal and maximal
   values these dimensions can take are bound by `min_positions` and
   `max_positions`.
 */
void pen_initialise_population(float* population,
                               size_t colony_size,
                               size_t dim,
                               const float min_position,
                               const float max_position);

/**
   Get the initial fitness values for each penguin in the population. This will
   generate a new fitness array.
 */
void pen_update_fitness(float* fitness,
                        size_t colony_size,
                        size_t dim,
                        const float * population,
                        obj_func_t obj_func);

/**
   Compute the euclidean distance between two penguins.
 */
float pen_eucledian_distance(size_t dim,
                              const float * penguin_i,
                              const float * penguin_j);

/**
   Computes the heat radiation of a penguin. See equation 6 in paper.
 */
float pen_heat_radiation();

/**
   Get the attractiveness between two penguins. Uses the heat radiation and attenuation
   coefficient. Note that the heat radiation determines the direction of attractiveness.
   If `heat_rad` refers to the heat radiation of `penguin_i`, then the attractiveness is
   how much `penguin_j` is attracted to `penguin_i`, not the other way around.
 */
float pen_attractiveness(float heat_rad,
                          size_t dim,
                          const float * penguin_i,
                          const float * penguin_j,
                          float attenuation_coef);

/**
   Compute spiral movement of the penguin towards the centre. See paper on SPO for
   clarification.
 */
void pen_get_spiral_like_movement(float* spiral, float attract, size_t dim,
                                  const float * centre, const float * penguin,
                                  const float* rotation_matrix);

/**
   Initialise the rotation matrix of size `dim` times `dim`. Its rotation rate is given by
   `theta` (between -pi and pi).
 */
void  pen_init_rotation_matrix(float* matrix, size_t dim, const float theta);

/**
   Mutates the spiral according the equation 19 from the paper. This modifies the spiral
   in place.
 */
void pen_mutate(size_t dim, float * spiral, float mutation_coef);

/**
   Clamps the solution in the possible range. This is done in place.
 */
void pen_clamp_position(size_t dim, float * position,
                        const float min_position,
                        const float max_position);

/**
   Gets the index of the fittest penguin in the population. Note that the less fit, the better.
 */
size_t pen_get_fittest_idx(size_t colony_size, const float *  fitness);

#ifdef __cplusplus
}
#endif // __cplusplus
