#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>

#include "pso.h"
#include "utils.h"


#define EPS 0.001
#define COG 2
#define SOCIAL 2
#define INERTIA 0.5
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

//returns swarm_size * dim size array
double* rand_init(size_t swarm_size, size_t dim,
									const double* const min_positions,
									const double* const max_positions){
	double* init = (double*)malloc(swarm_size*dim*sizeof(double));
	for (size_t particle=0; particle < swarm_size; particle++){
			for (size_t d=0; d<dim; d++){
					size_t idx = (particle*dim) + d;
					// init[idx] = random_min_max(min_positions[d],max_positions[d]);
					init[idx] = min_positions[d] + (max_positions[d] - min_positions[d])*((double)rand() / RAND_MAX);
			}
	}
	return init;
}

//returns swarm_size size array
void eval_fitness(double (*func)(const double* const, size_t),
									size_t swarm_size, size_t dim,
									const double* const x, double* fitness){
	for (size_t pop=0; pop<swarm_size; pop++){
			fitness[pop] = func(x+(pop*dim),dim);
	}
	return ;
}

//returns swarm_size x dim size array
// velocity at every posize_t of every particle
double* gen_init_velocity(const double* const x,
													size_t swarm_size, size_t dim,
													const double* const min_positions,
													const double* const max_positions){
		double* u = (double *)malloc(swarm_size*dim*sizeof(double));
		u = rand_init(swarm_size,dim,min_positions,max_positions);
		double* v = (double *)malloc(swarm_size*dim*sizeof(double));
		for (size_t particle=0; particle < swarm_size; particle++){
				for (size_t d=0; d<dim; d++){
						size_t idx = (particle*dim) + d;
						v[idx] = 0.25*(u[idx] - x[idx]);
				}
		}
		return v;
}

// sorts the population in decreasing order of fitness /
// i.e pop[0] posize_ts to best location yet.
size_t best_fitness(double* fitness,size_t dim, size_t swarm_size){
	// find minimum of fitness
	// set index of mimima as minima_idx;
	// global_best = x+(dim*minima_idx);
	size_t minima_idx = 0;
	double best_yet = fitness[0]; // global best at timestep t+1
	for (size_t p=1;p<swarm_size;p++){
			if (fitness[p]<best_yet) {
					best_yet = fitness[p];
					minima_idx = p;
			}
	}
	// global_best_position = local_best_position+(dim*minima_idx);
	return minima_idx;
}

void generate_vel_limit(const double* min_positions, const double* max_positions,
												double* min_vel, double* max_vel,size_t dim){
		for (size_t d=0;d<dim;d++){
				min_vel[d] = min_positions[d]/5;
				max_vel[d] = max_positions[d]/5;
		}
}

// updates velocities, works on array returned by gen_velocity
double* update_velocity(double * v, double* x, double * y,
												double* best, size_t swarm_size, size_t dim,
												const double* const min_vel,
												const double* const max_vel){
	double* vnew = (double*)malloc(dim*swarm_size*sizeof(double));
	for (size_t particle=0; particle < swarm_size; particle++){
			for (size_t d=0; d<dim; d++){
					size_t idx = (particle*dim) + d;
					vnew[idx] = INERTIA*v[idx] + COG*((double)rand()/RAND_MAX)*(y[idx] - x[idx]) +
															SOCIAL*((double)rand()/RAND_MAX)*(best[d] * x[idx]);
					vnew[idx] = INERTIA*v[idx] + COG*random_0_to_1()*(y[idx] - x[idx]) +
															SOCIAL*random_0_to_1()*(best[d] * x[idx]);
					vnew[idx] = min(max(min_vel[d],vnew[idx]),max_vel[d]);
			}
	}
	return vnew;
}

void update_position( double* x, double* v,
										  size_t swarm_size, size_t dim,												const double* const min_positions,
											const double* const max_positions){
	for (size_t particle=0; particle < swarm_size; particle++){
			for (size_t d=0; d<dim; d++){
					size_t idx = (particle*dim) + d;
					x[idx] += v[idx];
					if (x[idx] >= max_positions[d] || x[idx] <= min_positions[d]){
						v[idx] = -v[idx];
					}
					x[idx] = min(max(min_positions[d],x[idx]),max_positions[d]);
					//TODO: PROJECT ON BOUNDS - RATHER DO A BOUNCE THAN A SIMPLE PROJECT
			}
	}
	return;
}


/**
Particle Swarm Algorithm to find the global minima of objective function using a nature mimicking swarm */
double * pso_basic (double(*fit)(const double* const, size_t), size_t swarm_size,
												size_t dim, size_t max_iter,
                        const double* const min_positions,
                        const double* const max_positions){

	srand((unsigned) time(NULL));
	// REPLACE WITH FIXED ZERO SEED

	// double known_optima = 0.; 	// TODO : DEPENDENT ON OBJECTIVE FUNCTION
	double* min_vel = (double*)malloc(dim*sizeof(double));
	double* max_vel = (double*)malloc(dim*sizeof(double));
	generate_vel_limit(min_positions,max_positions,min_vel,max_vel,dim);

	size_t sizeof_position = dim*swarm_size*sizeof(double);
	// randomly initialise postions of swarm particles
	double* current_positions = (double*)malloc(sizeof_position);
	current_positions = rand_init(swarm_size, dim,min_positions,max_positions);
	double* local_best_position = (double*)malloc(sizeof_position);
	memcpy(local_best_position,current_positions, sizeof_position);

	// calculate the fitness at every position of the swarm
	size_t sizeof_fitness = swarm_size*sizeof(double);
	double* current_fitness = (double*)malloc(sizeof_fitness);
	eval_fitness(fit,swarm_size,dim,current_positions,current_fitness);
	double* local_best_fitness = (double*)malloc(sizeof_fitness);
	memcpy(local_best_fitness,current_fitness, sizeof_fitness );

	double* p_velocity = (double*)malloc(sizeof_position);
	p_velocity = gen_init_velocity(current_positions,swarm_size,dim,
																	min_positions,max_positions);

	double* global_best_position = (double*)malloc(dim*sizeof(double));
	size_t global_best_idx = best_fitness(local_best_fitness,dim,swarm_size);

	global_best_position = (local_best_position + (dim*global_best_idx));
	double global_best_fitness = local_best_fitness[global_best_idx];

	size_t iter = 0;
	// double error = 1e3;

	while ( iter < max_iter) { // error > EPS &&

			p_velocity = update_velocity(p_velocity,current_positions,
									local_best_position,global_best_position,swarm_size,
									dim,min_vel,max_vel);

			update_position(current_positions,p_velocity,swarm_size,
											dim,min_positions,max_positions);

			eval_fitness(fit,swarm_size,dim,current_positions,current_fitness);

			// check local best fitness
			for (size_t p=0;p<swarm_size;p++){
					if (current_fitness[p] < local_best_fitness[p]){
							local_best_fitness[p] = current_fitness[p];
							for (int d=0;d<dim;d++){
									size_t idx = (p*dim) + d;
									local_best_position[idx] = current_positions[idx];
							}
					}
			}

			global_best_idx = best_fitness(local_best_fitness,dim,swarm_size);
			global_best_position = local_best_position+(dim*global_best_idx);
			global_best_fitness  = local_best_fitness[global_best_idx];
			iter++;

	}

	return global_best_position;
}


/*****************************
UNIT TESTS
*****************************/
// test function
/*
static double sum_of_squares(const double* const args, size_t dim) {
	double sum = 0.0;
	for(size_t idx = 0; idx < dim; idx++) {
		sum += pow(args[idx], 2.0);
	}
	return sum;
}

Test(pso_unit,rand_init){
	srand((unsigned) time(NULL));
	size_t swarm_size = 10;
	size_t dim = 4;
	double mins[] = {0.0 , -1 , 1.0 , -10.0};
	double maxs[] = {1.0 , 1.0, 3.0 , 25.0};
	double* const pos = rand_init(swarm_size,dim,mins,maxs);
	for (size_t s=0;s<swarm_size;s++){
		cr_assert(pos[s * dim] <= maxs[0], "first dimension should be bound above");
    cr_assert(pos[s * dim] >= mins[0], "first dimension should be bound below");
    cr_assert(pos[s * dim + 1] <= maxs[1], "second dimension should be bound above");
    cr_assert(pos[s * dim + 1] >= mins[1], "second dimension should be bound below");
    cr_assert(pos[s * dim + 2] <= maxs[2], "third dimension should be bound above");
    cr_assert(pos[s * dim + 2] >= mins[2], "third dimension should be bound below");
    cr_assert(pos[s * dim + 3] <= maxs[3], "fourth dimension should be bound above");
    cr_assert(pos[s * dim + 3] >= mins[3], "fourth dimension should be bound below");

	}
}


Test(pso_unit,eval_fitness){
	size_t swarm_size, dim;
	swarm_size = 4;
	dim = 2;
	double x[] = {
		 0.0,  0.0,
		 2.5,  4.0,
		10.0,  3.0,
	 -10.0, -3.0
	};
	double * fitness = (double*)malloc(sizeof(double)*swarm_size);
	eval_fitness(sum_of_squares, swarm_size, dim, x, fitness);
	cr_assert(fabs(fitness[0] - 0.0) < DBL_EPSILON, "first particle fitness should be 0");
	cr_assert(fabs(fitness[1] - 22.25) < DBL_EPSILON, "second particle fitness should be 17");
  cr_assert(fabs(fitness[2] - 109.0) < DBL_EPSILON, "third wolf's fitness should be 25");
  cr_assert(fabs(fitness[3] - 109.0) < DBL_EPSILON, "fourth wolf's fitness should be 13");

}

Test(pso_unit,gen_init_velocity){
	size_t swarm_size, dim;
	swarm_size = 4;
	dim = 2;
	double mins[] = {0.0 , -1 , 1.0 , -10.0};
	double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

	double* const x = rand_init(swarm_size,dim,mins,maxs);

	double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
	vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
	for (size_t s=0;s<swarm_size;s++){
			for (size_t d = 0; d < dim; d++) {
					size_t idx = s*dim + d;
			    cr_assert(vel[idx] <= (maxs[d] - x[idx])* 0.5,"%ld dimension should be bound above",d);
			    cr_assert(vel[idx] >= (mins[d] - x[idx])*0.5, "%ld dimension should be bound below",d);
			}
	}
}

Test(pso_unit,best_fitness){
	size_t swarm_size,dim;
	swarm_size = 4;
	dim = 2;
	double local_best_position[] = {
		100.0,  0.0,
		2.5,  4.0,
		10.0,  3.0,
	 -10.0, -3.0
	};

	double * fitness = (double*)malloc(sizeof(double)*swarm_size);
	eval_fitness(sum_of_squares, swarm_size, dim, local_best_position, fitness);

	double* global_best_position = (double*)malloc(sizeof(double)*dim);
	size_t best_index;
	best_index = best_fitness(fitness,dim, swarm_size);
  cr_assert(best_index == 1, "the optima is at local_best_position[1]");
	global_best_position = local_best_position+(dim*best_index);
	// printf("global_best_position[0]:%f\n", global_best_position[0] );
	// printf("global_best_position[1]:%f\n", global_best_position[1] );
  cr_assert(global_best_position[0] == 2.5, "first dim of global best is 1");
  cr_assert(global_best_position[1] == 4.0, "second dim of global best is 0");

	free(fitness);
}

Test(pso_unit,update_velocity){
	size_t swarm_size, dim;
	swarm_size = 4;
	dim = 2;
	double mins[] = {0.0 , -1 , 1.0 , -10.0};
	double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

	double* const x = rand_init(swarm_size,dim,mins,maxs);
	double* const y = rand_init(swarm_size,dim,mins,maxs);
	double* const best = rand_init(1,dim,mins,maxs);

	double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
	vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
	vel = update_velocity(vel, x, y, best, swarm_size, dim,
												mins, maxs);
	for (size_t s=0;s<swarm_size;s++){
			for (size_t d = 0; d < dim; d++) {
					size_t idx = s*dim + d;
			    cr_assert(vel[idx] <= maxs[d] ,"%ld dimension'd velocity should be bound above",d);
			    cr_assert(vel[idx] >= mins[d] , "%ld dimension'd velocity should be bound below",d);
			}
	}

	free(vel);
}

Test(pso_basic,update_position){
	size_t swarm_size, dim;
	swarm_size = 4;
	dim = 2;
	double mins[] = {0.0 , -1 , 1.0 , -10.0};
	double maxs[] = {1.0 , 1.0, 3.0 , 25.0};

	double* const x = rand_init(swarm_size,dim,mins,maxs);
	double* const y = rand_init(swarm_size,dim,mins,maxs);
	double* const best = rand_init(1,dim,mins,maxs);

	double* vel = (double*)malloc(sizeof(double)*swarm_size*dim);
	vel = gen_init_velocity(x,swarm_size,dim,mins,maxs);
	vel = update_velocity(vel, x, y, best, swarm_size, dim,
												mins, maxs);
	update_position( x, vel, swarm_size, dim, mins, maxs);
	for (size_t s=0;s<swarm_size;s++){
			for (size_t d = 0; d < dim; d++) {
					size_t idx = s*dim + d;
					cr_assert(x[idx] <= maxs[d] ,"%ld dimension'd position should be bound above",d);
					cr_assert(x[idx] >= mins[d] ,"%ld dimension'd position should be bound below",d);
			}
	}
}*/
/*
Test(pso_unit,pso_basic){

	size_t swarm_size = 50;
  size_t dim = 10;
  size_t max_iter = 2000;
  double* mins = filled_array(dim, -5);
  double* maxs = filled_array(dim, 5);
  double* solution = pso_basic(rastigrin, swarm_size, dim, max_iter, mins, maxs);

	cr_assert(fabs(rastigrin(solution, dim)) < 0.001, "objective should be minimised at 0");
  // free(solution);
  // free(mins);
  // free(maxs);
}
*/
