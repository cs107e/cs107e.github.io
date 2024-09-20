#ifndef RAND_H
#define RAND_H

/*
 * Module to generate random numbers.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 */

/*
 * `rand`
 *
 * Generate a 32-bit random number. Uses a psuedo-random
 * generator with a fixed seed.
 *
 * @return   pseudo-random value in the range 0 to UINT_MAX
 */
unsigned int rand(void);

#endif
