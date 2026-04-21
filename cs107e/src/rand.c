/*
 * Pseudorandom number generator
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "rand.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>

static struct {
    bool initialized;
    uint32_t z1, z2, z3, z4;
} module = {
    .initialized = false,
};

unsigned int rand(void) {
    if (!module.initialized) {
        srand(12345);
    }
    // http://stackoverflow.com/questions/1167253/implementation-of-rand
    // LFSR113 from L'écuyer
    uint32_t b;
    b = ((module.z1 << 6) ^ module.z1) >> 13;
    module.z1 = ((module.z1 & 4294967294U) << 18) ^ b;
    b = ((module.z2 << 2) ^ module.z2) >> 27;
    module.z2 = ((module.z2 & 4294967288U) << 2) ^ b;
    b = ((module.z3 << 13) ^ module.z3) >> 21;
    module.z3 = ((module.z3 & 4294967280U) << 7) ^ b;
    b = ((module.z4 << 3) ^ module.z4) >> 12;
    module.z4 = ((module.z4 & 4294967168U) << 13) ^ b;
    return (module.z1 ^ module.z2 ^ module.z3 ^ module.z4);
}

void srand(unsigned int seed) {
    // initial factors must be >= 2, 8, 16, and 128 respectively
    // this just lifts all to at least 128 to satisfy requirement
    if (seed < 128) seed += 128;
    module.z1 = seed;
    module.z2 = seed;
    module.z3 = seed;
    module.z4 = seed;
    module.initialized = true;
}