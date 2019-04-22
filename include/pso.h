#pragma once

#ifdef __cplusplus
extern "C" {
#endif

double * pso_basic (double(*fit)(const double* const, size_t),
                    size_t swarm_size,
                    size_t dim, size_t max_iter,
                    const double* const min_positions,
                    const double* const max_positions);

//returns swarm_size * dim size array
double* rand_init(size_t swarm_size,
                  size_t dim,
                  const double* const min_positions,
                  const double* const max_positions);

//returns swarm_size size array
void eval_fitness(double (*func)(const double* const, size_t),
                  size_t swarm_size,
                  size_t dim,
                  const double* const x,
                  double* fitness);

//returns swarm_size x dim size array
// velocity at every posize_t of every particle
double* gen_init_velocity(const double* const x,
                          size_t swarm_size, size_t dim,
                          const double* const min_positions,
                          const double* const max_positions);

// sorts the population in decreasing order of fitness /
// i.e pop[0] posize_ts to best location yet.
size_t best_fitness(double* fitness,size_t dim, size_t swarm_size);


void generate_vel_limit(const double* min_positions,
                        const double* max_positions,
                        double* min_vel,
                        double* max_vel,
                        size_t dim);

// updates velocities, works on array returned by gen_velocity
double* update_velocity(double* v,
                        double* x,
                        double* y,
                        double* best,
                        size_t swarm_size,
                        size_t dim,
                        const double* const min_vel,
                        const double* const max_vel);


void update_position(double* x,
                    double* v,
                    size_t swarm_size,
                    size_t dim,
                    const double* const min_positions,
                    const double* const max_positions);
#ifdef __cplusplus
}
#endif
