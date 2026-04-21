#ifndef RAND_H
#define RAND_H

/*
 * Module to generate random numbers.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 */

/*
 * `rand`
 *
 * Generate a 32-bit random number. Uses a psuedo-random
 * generator with a fixed seed by default.
 *
 * @return   pseudo-random value in the range 0 to UINT_MAX
 */
unsigned int rand(void);

/*
 * `srand`
 *
 * Use argument as seed for sequence of pseudo-random numbers to be
 * returned by rand().  Sequence is repeatable by calling srand()
 * with the same seed value.
 */
void srand(unsigned int seed);

#endif
