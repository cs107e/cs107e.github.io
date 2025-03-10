#ifndef TONE_H
#define TONE_H

/*
 * Hardware abstractions making a sound on the Raspberry Pi audio jack.
 *
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Feb 15, 2016
 */

/*
 * Default audio base frequency = 1 Mhz
 */
#define F_AUDIO 1000000

/*
 * generate a square wave with the given frequency.
 */
void tone(int freq);

#endif
