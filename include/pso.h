#pragma once

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

float * pso_basic (obj_func_t obj_func,
                    size_t swarm_size,
                    size_t dim, size_t max_iter,
                    const float min_position,
                    const float max_position);

//returns swarm_size * dim size array
void pso_rand_init(float* const population,
                  size_t swarm_size,
                  size_t dim,
                  const float min_position,
                  const float max_position);

//returns swarm_size size array
void pso_eval_fitness(obj_func_t obj_func,
                      size_t swarm_size,
                      size_t dim,
                      const float* positions,
                      float* fitness);

//returns swarm_size x dim size array
// velocity at every posize_t of every particle
void pso_gen_init_velocity(float* const velocity,
                          const float* positions,
                          size_t swarm_size, size_t dim,
                          const float min_position,
                          const float max_position);

// sorts the population in decreasing order of fitness /
// i.e pop[0] posize_ts to best location yet.
size_t pso_best_fitness(float* fitness,size_t dim, size_t swarm_size);


void pso_generate_vel_limit(const float min_position,
                            const float max_position,
                            float* min_vel,
                            float* max_vel);

// updates velocities, works on array returned by gen_velocity
void pso_update_velocity(float* velocity,
                         float* positions,
                         float* local_best_position,
                         float* best,
                         size_t swarm_size,
                         size_t dim,
                         const float min_vel,
                         const float max_vel);

void pso_update_position(float* positions,
                         float* velocity,
                         size_t swarm_size,
                         size_t dim,
                         const float min_position,
                         const float max_position);
#ifdef __cplusplus
}
#endif
