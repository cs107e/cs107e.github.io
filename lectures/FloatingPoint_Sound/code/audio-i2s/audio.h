#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include "i2s.h"

void audio_init(int sample_freq, int block_alignment, CHANNEL_TYPE ct);

void mic_init();

// these functions do not return if repeat is true
void audio_write_i16(const uint16_t waveform[], unsigned num_samples, int mono, int repeat);
void audio_write_i16_stereo_mix(const uint16_t waveform1[], const uint16_t waveform2[], unsigned num_samples, int repeat);

void audio_write_i16_dma(uint16_t waveform[], unsigned int num_samples, int repeat);

void mic_capture_dma(uint32_t *audio_samples, unsigned int num_samples);

#endif
