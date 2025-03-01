#ifndef _I2S_H
#define _I2S_H

/*
 * Hardware abstractions for pulse width modulation (I2S) of
 * digital signals.
 *
 * Author: Chris Gregg <cgregg@stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdint.h>

typedef enum {
    I2S_MONO = 0,
    I2S_STEREO = 1,
} i2s_frame_type_t;

void i2s_init(int sample_frequency);

void i2s_play_stream(int sample_freq, const int16_t data[], int ndata, i2s_frame_type_t ftype);

#endif
