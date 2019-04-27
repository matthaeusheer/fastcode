#pragma once

#ifdef __cplusplus
extern "C" {
#endif

double * pso_basic (double(*fit)(const double* const, size_t),
                    size_t swarm_size,
                    size_t dim, size_t max_iter,
                    const double* min_positions,
                    const double* max_positions);

//returns swarm_size * dim size array
double* pso_rand_init(size_t swarm_size,
                  size_t dim,
                  const double* min_positions,
                  const double* max_positions);

//returns swarm_size size array
void pso_eval_fitness(double (*func)(const double* const, size_t),
                  size_t swarm_size,
                  size_t dim,
                  const double* positions,
                  double* fitness);

//returns swarm_size x dim size array
// velocity at every posize_t of every particle
double* pso_gen_init_velocity(const double* positions,
                          size_t swarm_size, size_t dim,
                          const double* min_positions,
                          const double* max_positions);

// sorts the population in decreasing order of fitness /
// i.e pop[0] posize_ts to best location yet.
size_t pso_best_fitness(double* fitness,size_t dim, size_t swarm_size);


void pso_generate_vel_limit(const double* min_positions,
                        const double* max_positions,
                        double* min_vel,
                        double* max_vel,
                        size_t dim);

// updates velocities, works on array returned by gen_velocity
void pso_update_velocity(double* velocity,
                        double* positions,
                        double* local_best_position,
                        double* best,
                        size_t swarm_size,
                        size_t dim,
                        const double* min_vel,
                        const double* max_vel);

void pso_update_position(double* positions,
                         double* velocity,
                         size_t swarm_size,
                         size_t dim,
                         const double* min_positions,
                         const double* max_positions);
#ifdef __cplusplus
}
#endif
