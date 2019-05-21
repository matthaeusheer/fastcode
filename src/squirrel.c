#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <float.h>
#include <limits.h>

#include "squirrel.h"
#include "utils.h"

#define NUM_JUMP_HICK 0.125
#define T_MAX 100
#define PREDATOR_PROB 0.1
#define BETA 1.5
#define GLIDING_CONST 1.9
#define CD 0.60
#define CL_MIN 0.675
#define CL_MAX 1.5
#define SF 18
#define DROP 8

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef SQRT_PI
#define SQRT_PI 1.77245385091
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


__m256i seed_a;
__m256i seed_b;
__m256 max_rng;

__m256 factor_0_to_1;
__m256 middle_0_to_1;

__m256 drop_invSFdrag;
__m256 gliding_const;

float basic_drop_invSFdrag;
/**
   Seed a parallel floating point RNG.
 */
void sqr_seed_simd_rng(size_t seed) {
  srand(seed);
  seed_a = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());
  seed_b = _mm256_set_epi32(rand(), rand(), rand(), rand(),
                            rand(), rand(), rand(), rand());
  max_rng = _mm256_cvtepi32_ps(_mm256_set1_epi32(INT_MIN));

  factor_0_to_1 = _mm256_set1_ps(0.5);
  middle_0_to_1 = _mm256_set1_ps(0.5);

  __m256 drag = _mm256_set1_ps(CD);
  __m256 drop = _mm256_set1_ps(DROP);
  __m256 sf   = _mm256_set1_ps(SF);

  __m256 invSFdrag = _mm256_mul_ps(sf,drag);
  drop_invSFdrag = _mm256_div_ps(drop,invSFdrag);
  basic_drop_invSFdrag = DROP/(SF*CD);
  gliding_const = _mm256_set1_ps(GLIDING_CONST);
}

/**
   Generate a vector of random floats between `min` and `max`.
 */
__m256 simd_rand_min_max(float min, float max) {
  float factor = (max - min) / 2;
  float middle = min + factor;

  __m256i s1 = seed_a;
	const __m256i s0 = seed_b;
	seed_a = seed_b;
	s1 = _mm256_xor_si256(seed_b, _mm256_slli_epi64(seed_b, 23));
	seed_b = _mm256_xor_si256(_mm256_xor_si256(_mm256_xor_si256(s1, s0),
                                             _mm256_srli_epi64(s1, 18)),
                            _mm256_srli_epi64(s0, 5));
  __m256i rands = _mm256_add_epi64(seed_b, s0);
  __m256 frands = _mm256_div_ps(_mm256_cvtepi32_ps(rands), max_rng);
  __m256 factors = _mm256_set1_ps(factor);
  __m256 middles = _mm256_set1_ps(middle);
  return _mm256_fmadd_ps(factors, frands, middles);
}

/**
   Generate a vector of random floats between 0 and 1.
*/
__m256 simd_rand_0_to_1() {
  __m256i s1 = seed_a;
	const __m256i s0 = seed_b;
	seed_a = seed_b;
	s1 = _mm256_xor_si256(seed_b, _mm256_slli_epi64(seed_b, 23));
	seed_b = _mm256_xor_si256(_mm256_xor_si256(_mm256_xor_si256(s1, s0),
                                             _mm256_srli_epi64(s1, 18)),
                            _mm256_srli_epi64(s0, 5));
  __m256i rands = _mm256_add_epi64(seed_b, s0);
  __m256 frands = _mm256_div_ps(_mm256_cvtepi32_ps(rands), max_rng);
  return _mm256_fmadd_ps(factor_0_to_1, frands, middle_0_to_1);
}
/**
    generate length size array of random numbers in range (min,max)
*/
void sqr_rand_init(float* const positions,
                      size_t length,
                      const float min,
                      const float max) {
  size_t idx = 0;
  if (length > 7) {
    for ( ; idx < length - 8; idx+=8){
      _mm256_storeu_ps(&positions[idx], simd_rand_min_max(min, max));
    }
  }
  for(; idx < length; idx++) {
    positions[idx] = random_min_max(min, max);
  }
}


int sqr_bernoulli_distribution(float probability){
  if (probability < 0 || probability > 1)
    return -1;

  if (random_0_to_1() < probability )
    return 1;
  return 0;
}

void sqr_eval_fitness(obj_func_t obj_func,
                      size_t pop_size, size_t dim,
                      const float* const positions,
                      float* fitness) {
  for (size_t pop=0; pop<pop_size; pop++){
    fitness[pop] = obj_func(positions+(pop*dim),dim);
  }
}


void sqr_lowest_val_to_front(float* fitness,float* positions, size_t pop_size, size_t dim){
  float best_yet = fitness[0];
  size_t best_idx = 0;

  for (size_t pop_idx=1; pop_idx<pop_size; pop_idx++){
    if (fitness[pop_idx] < best_yet){
      best_yet = fitness[pop_idx];
      best_idx = pop_idx;
    }
  }

  float temp = fitness[0];
  fitness[0] = fitness[best_idx];
  fitness[best_idx] = temp;

  // careful
  float temp_pos[dim];

  memcpy(temp_pos, positions, dim*sizeof(float));
  memcpy(positions, positions+(dim*best_idx), dim*sizeof(float));
  memcpy(positions +(dim*best_idx), temp_pos, dim*sizeof(float));

  // free(temp_pos);
}

  /**
  *   sort the fitness and position arrays
  *   so that the best four squirrels
  *   are the first four elements
  **/
void sqr_lowest4_vals_to_front(float* fitness,float* positions, size_t pop_size, size_t dim){
  for (size_t i = 0; i < 4; i++) {
    sqr_lowest_val_to_front(fitness+i,positions+i*dim,pop_size-i,dim);
  }
}

__m256 sqr_simd_gliding_dist(){
  __m256 lift = simd_rand_min_max(CL_MIN, CL_MAX);
  return _mm256_mul_ps(drop_invSFdrag,lift);
}

float sqr_gliding_dist(){
  float lift = random_min_max(CL_MIN,CL_MAX);
  return basic_drop_invSFdrag*lift;
}

void sqr_move_to_hickory(float* positions,
                    size_t pop_size,
                    size_t dim,
                    const float min_position,
                    const float max_position){

  if (!sqr_bernoulli_distribution(PREDATOR_PROB)){
    for (size_t pop_idx = 1; pop_idx < 4+NUM_JUMP_HICK*pop_size ; pop_idx ++){
      size_t d = 0;
      if (dim > 7 ){
        for ( ; d < dim - 8; d+=8){
          size_t idx = pop_idx*dim + d;

          __m256 gl_dist = sqr_simd_gliding_dist();
          __m256 simd_pos = _mm256_loadu_ps( &positions[idx]);
          __m256 simd_best_pos = _mm256_loadu_ps(&positions[dim]);

          __m256 glide = _mm256_mul_ps(gliding_const,gl_dist);
          __m256 diff = _mm256_sub_ps(simd_best_pos,simd_pos);
          __m256 new_pos = _mm256_fmadd_ps(glide,diff,simd_pos);

          _mm256_storeu_ps(&positions[idx],new_pos);
        }
      } else {
          for (size_t d = 0; d < dim; d++){
            size_t idx = pop_idx*dim + d;
            positions[idx] = positions[idx] +
            sqr_gliding_dist()*GLIDING_CONST*(positions[d]-positions[idx]);
          }
        }
      }
  } else {
    for (size_t pop_idx = 1; pop_idx < 4+NUM_JUMP_HICK*pop_size ; pop_idx ++){
      size_t d = 0;
      if (dim > 7) {
        for ( ; d < dim - 8; d+=8){
          size_t idx = pop_idx*dim + d;
          _mm256_storeu_ps(&positions[idx],simd_rand_min_max(min_position,max_position));
        }
      } else {
        for (size_t d = 0; d < dim; d++){
          size_t idx = pop_idx*dim + d;
          positions[idx] = random_min_max(min_position,max_position);
        }
      }
    }
  }
  return;
}

void sqr_move_normal_to_acorn(float* positions,
                          size_t pop_size,
                          size_t dim,
                          const float min_position,
                          const float max_position){
  float p = PREDATOR_PROB;
  if (!sqr_bernoulli_distribution(p)){
    for (size_t pop_idx = 4+NUM_JUMP_HICK*pop_size; pop_idx < pop_size; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        size_t acorn_idx = ( 1 + ( idx % 3))*dim + d;
        positions[idx] = positions[idx] +
                        sqr_gliding_dist()*GLIDING_CONST*(positions[acorn_idx]-positions[idx]);
      }
    }
  } else {
    for (size_t pop_idx = 4+NUM_JUMP_HICK*pop_size; pop_idx < pop_size; pop_idx ++){
      for (size_t d = 0; d < dim; d++){
        size_t idx = pop_idx*dim + d;
        positions[idx] = random_min_max(min_position,max_position);
      }
    }
  }
  return;
}

float sqr_eval_seasonal_cons(float* positions, size_t dim){
  float s_c_2 = 0;
  for (size_t pop_idx = 1; pop_idx < 4; pop_idx ++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = pop_idx*dim + d;
      s_c_2 += pow( (positions[idx] - positions[d]),2);
    }
  }
  return sqrt(s_c_2);
}

float sqr_eval_smin(size_t iter){
return 1e-6/( pow( 365, ( 2.5*iter/T_MAX)));
}

float sqr_factorial(size_t n){
  if (n>=1) return n*sqr_factorial(n-1);
  else return 1;
}

float sqr_eval_gamma(float x){
  if (x >= 1) {
  size_t n = floor(x);
  return SQRT_PI*
        ( sqr_factorial(2*n-1) )/( sqr_factorial(n-1)*pow(2,(2*n-1)) );
  } else {
    size_t n = fabs(floor(x));
    return pow(-4,n)*sqr_factorial(n)*SQRT_PI/sqr_factorial(2*n);
  }
}

float sqr_levy_flight(){
  float sigma = pow( ( ( sqr_eval_gamma( 1+BETA)*sin( 0.5*M_PI*BETA))/
                 ( sqr_eval_gamma( 0.5*( 1+BETA))*BETA*pow( 2,( 0.5*( BETA-1))))),( 1/BETA));

  return 0.01*random_0_to_1()*sigma/pow( random_0_to_1(),( 1/BETA) );
}

void random_restart(float* positions,size_t pop_size, size_t dim, const float min_position, const float max_position){
  float range = max_position - min_position;
  for (size_t pop_idx = 4+NUM_JUMP_HICK*pop_size; pop_idx < pop_size; pop_idx ++){
    for (size_t d = 0; d < dim; d++){
      size_t idx = pop_idx*dim + d;
      positions[idx] =  min_position + sqr_levy_flight()*(range);
    }
  }
  return;
}


float* squirrel (obj_func_t obj_func,
                  size_t pop_size,
                  size_t dim,
                  size_t max_iter,
                  const float min_position,
                  const float max_position) {
  sqr_seed_simd_rng(100);

  // float p_dp = PREDATOR_PROB;
  // size_t num_jump_hick = ceil(NUM_JUMP_HICK*pop_size);

  size_t sizeof_position = pop_size*dim*sizeof(float);
  float* positions = (float*)malloc(sizeof_position);
  if (!positions) { perror("malloc arr"); exit(EXIT_FAILURE); };
  sqr_rand_init(positions,pop_size*dim,min_position,max_position);

  size_t sizeof_fitness = pop_size*sizeof(float);
  float* fitness = (float*)malloc(sizeof_fitness);
  if (!fitness) { perror("malloc arr"); exit(EXIT_FAILURE); };
  sqr_eval_fitness(obj_func,pop_size,dim,positions,fitness);

  // positions[0] is hickory, positions[1:3] are acorn, rest are normal.
  sqr_lowest4_vals_to_front(fitness,positions,pop_size,dim);

  #ifdef DEBUG
    print_population(pop_size, dim, positions); // printing the initial status of the population
    printf("# AVG FITNESS: %f\n", average_value(pop_size, fitness));
    printf("# BEST FITNESS: %f\n", lowest_value(pop_size, fitness));
  #endif

  float s_c = 0;
  size_t iter = 0;
  float s_min = sqr_eval_smin(iter); // seasonal constant
  while (iter < max_iter) {
    iter++;

    sqr_move_to_hickory(positions,pop_size,dim,min_position,max_position);
    sqr_move_normal_to_acorn(positions,pop_size,dim,min_position,max_position);

    s_c = sqr_eval_seasonal_cons(positions, dim);
    if (s_c < s_min){
      random_restart(positions,pop_size,dim,min_position,max_position);
    }
    s_min = sqr_eval_smin(iter);

    sqr_eval_fitness(obj_func,pop_size,dim,positions,fitness);
    sqr_lowest4_vals_to_front(fitness,positions,pop_size,dim);

    #ifdef DEBUG
      print_population(pop_size, dim, positions); // printing the initial status of the population
      printf("# AVG FITNESS: %f\n", average_value(pop_size, fitness));
      printf("# BEST FITNESS: %f\n", lowest_value(pop_size, fitness));
    #endif
  }

  float* const best_solution = (float *const) malloc(dim*sizeof(float));
  if (!best_solution) { perror("malloc arr"); exit(EXIT_FAILURE); };
  memcpy(best_solution, positions , dim*sizeof(float));

  free(fitness);
  free(positions);

  return best_solution;
}
