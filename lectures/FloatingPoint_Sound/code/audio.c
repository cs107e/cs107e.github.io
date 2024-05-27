#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "i2s.h"
#include "audio.h"
#include "printf.h"
#include "mango.h"

/* Modified by Chris Gregg, May 2024 */

/*
   Initialize the i2s for audio.
   - base clock is 9600000
   - i2s range is base clock / sample_freq to give correct sample clock rate
*/

void audio_init(int sample_freq, int block_alignment) 
{
    i2s_setup(sample_freq, block_alignment);
    i2s_enable();
}

/* 
   These functions transmit a wave to the RPi audio jack 
   as a pulse-width-modulated signal.
   
   The waveform array contains 256 entries.

   There are 3 functions depending on the type of the
   waveform array: u8, u16, i16.

   phase is a 32-bit quantity. The upper 8-bits are used as an
   index into the waveform array and selects one of the 256 samples.
   
   dphase is added to phase after each sample is output.  

   Setting dphase=(1<<24) will output one sample per interval. 
   Running the i2s at 9600000 / 256 samples / 256 range results
   in the waveform being output at 146.5 Hz.

   Setting dphase=(2<<24) will skip one sample per interval,
   and will double the frequency of the output to 293Hz.
   
   If repeat is true, the functions will not return.
*/

void audio_write_i16(const int16_t waveform[], unsigned num_samples, int mono, int repeat) 
{
    // assume num_samples is even for stereo
    if (!mono && num_samples % 2 != 0) {
        mango_abort();
    }
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = i2s_get_status();
            while (status == 0) {
                status = i2s_get_status();
            }
            if (mono) {
                int16_t pcm = waveform[sample];
                i2s_write( pcm, pcm ); // output to left
            } else { // stereo
                int16_t pcm_left = waveform[sample];
                int16_t pcm_right = waveform[sample + 1];
                i2s_write( pcm_left, pcm_right ); // output to left
            }
        }
        printf("repeating\n");
        if (!repeat) break;
    }
}

void audio_write_i16_stereo_mix(const int16_t waveform1[], const int16_t waveform2[], unsigned num_samples, int repeat) 
{
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = i2s_get_status();
            while (status == 0) {
                status = i2s_get_status();
            }
            int16_t pcmL = waveform1[sample];
            int16_t pcmR = waveform2[sample];
            i2s_write( pcmL, pcmR );
        }
        printf("repeating\n");
        if (!repeat) break;
    }
}

