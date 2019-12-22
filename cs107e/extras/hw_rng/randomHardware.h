#ifndef RANDOMHARDWARE_H
#define RANDOMHARDWARE_H

/*
 *
 * Same random hardware generator I used for assignment 2.
 *
 * Functions for interfacing with the hardware RNG module.
 * Author: Essentially popcornmix
 *
 * Date: April 23rd
 *
 * Note: I did not come up with this, apparently there is a poorly documented
 * peripheral that does not appear in the broadcom manual that uses thermal noise
 * in the chip to generate random values.
 *
 * I picked through the official Rpi linux driver at:
 * https://github.com/raspberrypi/linux/blob/204050d0eafb565b68abf512710036c10ef1bd23/drivers/char/hw_random/bcm2835-rng.c
 * (courtesy of github user popcornmix) which is the primary source of information on the implementation
 * of the peripheral for all of the posts that I have seen.
 *
 * I took heavy inspiration from the bare metal implementation at: https://www.raspberrypi.org/forums/viewtopic.php?t=196015
 * (courtesy of Rpi forums user 'bzt') which does not include excess
 * linux support material.
 *
 * If you go to thse pages you will notice that my code is eerily similar.
 * Unfortunately there are no other sources of information about the use of this
 * peripheral other than the linux driver and derived code (see official description
 * of registers [all "todo"] at https://elinux.org/BCM2835_registers#RNG). If I had
 * enough information to build my own library I happily would.
 *
 * Once again I take no credit for the contents of this library. I decided to go
 * with this approach rather than some software based pseudorandom number generator
 * because I am curious about the peripherals on the chip and want to learn more
 * about the hardware. The conditions for the use of this library are that it only
 * be used for extensions because the code is derivative (though it is an integral
 * part of my extension XD) as per an email exchange with the course administrators.
 *
 * Who knows what other peripherals lay undocumented in the processor.
 */

/*
 * Initializes the random number generator
 */
void random_init();

/*
 * Retreives a random 32 bit value.
 */
unsigned int random_getNumber();

#endif
