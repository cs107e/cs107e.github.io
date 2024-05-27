#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void audio_init(int sample_freq, int block_alignment);

// these functions do not return if repeat is true
void audio_write_i16(const int16_t waveform[], unsigned num_samples, int mono, int repeat);
void audio_write_i16_stereo_mix(const int16_t waveform1[], const int16_t waveform2[], unsigned num_samples, int repeat);

#endif
