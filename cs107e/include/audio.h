#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void audio_init(int sample_freq);

// these functions do not return if repeat is true
void audio_write_u8 (const uint8_t  waveform[], unsigned num_samples, int repeat);
void audio_write_u16(const uint16_t waveform[], unsigned num_samples, int repeat);
void audio_write_i16(const  int16_t waveform[], unsigned num_samples, int repeat);

#endif
