#include "randomHardware.h"

// TODO randomHardware.c: does not seem to return scaled value.

//------------------------------ Constants ------------------------

// See the link in the header for registers, the descriptions are all "todo" though.
#define HWRNG_CTRL     ((volatile unsigned int *) 0x20104000) // Seems 1 to enable, 0 to disable
#define HWRNG_STATUS   ((volatile unsigned int *) 0x20104004) // Idk how this works
#define HWRNG_DATA     ((volatile unsigned int *) 0x20104008) // Provides 32 random bits
#define HWRNG_INT_MASK ((volatile unsigned int *) 0x20104010) // Used to set interrupt

#define RNG_INITIAL_DISCARDS 0x40000 // Borrowed from the linux driver, RNG needs to start up

//------------------------- Function Definitions ------------------

/*
 * Initializes the random number generator
 */
void random_init(void) {
  // Frankly I have no idea how the RNG_STATUS register works, details are
  // pretty fuzzy and this implementation is derivative of a derivative of the
  // original uncommented linux driver. I feel pretty confident about HWRNG_CTRL
  // though. I have no idea about the interrupt register.
  *HWRNG_CTRL = 1;                       // enables the random generator
  *HWRNG_STATUS = RNG_INITIAL_DISCARDS;   // sets accumulation
  *HWRNG_INT_MASK |= 1;                   // enables interrupt
  while(!((*HWRNG_STATUS) >> 24));          // waits until accumulation complete
}

/*
 * Returns a single random unsigned int.
 */
unsigned int random_getNumber(unsigned int max, unsigned int min) {
  return (((*HWRNG_DATA) % (max - min)) + min);
}
