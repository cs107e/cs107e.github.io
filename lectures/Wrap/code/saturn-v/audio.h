#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include "i2s.h"

void audio_init(int sample_freq, int block_alignment, CHANNEL_TYPE ct);

// these functions do not return if repeat is true
void audio_write_i16(const int16_t waveform[], unsigned num_samples, int mono, int repeat);
void audio_write_i16_stereo_mix(const int16_t waveform1[], const int16_t waveform2[], unsigned num_samples, int repeat);

void audio_write_i16_dma(int16_t waveform[], unsigned int num_samples, int repeat);

#endif
