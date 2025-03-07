#ifndef _I2S_H
#define _I2S_H

/*
 * I2S/PCM peripheral
 */

#include <stdint.h>
#include "dma.h"

typedef enum {
    I2S_MONO = 0,
    I2S_STEREO = 1,
} i2s_frame_type_t;

void i2s_init(int sample_frequency);

// blocking
void i2s_play_stream(const int16_t data[], int ndata, i2s_frame_type_t ftype);

// non-blocking
dma_channel_id_t i2s_play_stream_dma(const int16_t data[], int ndata, i2s_frame_type_t ftype);
bool i2s_dma_playback_complete(dma_channel_id_t);

#endif
