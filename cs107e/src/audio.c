#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "audio.h"

/* Modified by Chris Gregg, Oct. 2021 */

/*
   Initialize the pwm for audio.
   - base clock is 9600000
   - pwm range is base clock / sample_freq to give correct sample clock rate
*/
void audio_init(int sample_freq) 
{
    gpio_set_function( 40, GPIO_FUNC_ALT0 );
    gpio_set_function( 45, GPIO_FUNC_ALT0 );
    timer_delay_ms(2);

    pwm_init();
    int clock_rate = 19200000 / 2; // 9600000 Hz
    int range = clock_rate / sample_freq;

    pwm_set_clock( clock_rate );
    timer_delay_ms(2);

    pwm_set_mode( 0, PWM_SIGMADELTA ); 
    pwm_set_mode( 1, PWM_SIGMADELTA );

    pwm_set_fifo( 0, 1 ); 
    pwm_set_fifo( 1, 1 );

    pwm_enable( 0 ); 
    pwm_enable( 1 );

    pwm_set_range( 0, range );
    pwm_set_range( 1, range );

    timer_delay_ms(2);
}

/* 
   These functions transmit a wave to the RPi audio jack 
   as a pulse-width-modulated signal.
   
   The waveform array contains 256 entries.

   There are 3 functions depending on the type of the
   waveform array: u8, u16, i16.

   phase is a 32-bit quantity. The upper 8-bits are used as an
   index into the wavefrom array and selects one of the 256 samples.
   
   dphase is added to phase after each sample is output.  

   Setting dphase=(1<<24) will output one sample per interval. 
   Running the pwm at 9600000 / 256 samples / 256 range results
   in the waveform being output at 146.5 Hz.

   Setting dphase=(2<<24) will skip one sample per interval,
   and will double the frequency of the output to 293Hz.
   
   If repeat is true, the functions will not return.
*/

void audio_write_u8(const uint8_t waveform[], unsigned num_samples, int repeat) 
{
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = pwm_get_status();
            while (status & PWM_FULL1) {
                status = pwm_get_status();
            }
            uint8_t pcm = waveform[sample];
            // mono
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
        }
        if (!repeat) break;
    }
}

void audio_write_u16(const uint16_t waveform[], unsigned num_samples, int repeat) 
{
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = pwm_get_status();
            while (status & PWM_FULL1) {
                status = pwm_get_status();
            }
            uint8_t pcm = waveform[sample] >> 8;
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
        }
        if (!repeat) break;
    }
}

void audio_write_i16(const int16_t waveform[], unsigned num_samples, int repeat) 
{
    while (1) {
        for (unsigned int sample = 0; sample < num_samples; sample++) {
            unsigned status = pwm_get_status();
            while (status & PWM_FULL1) {
                status = pwm_get_status();
            }
            unsigned wave = waveform[sample] - INT16_MIN;
            uint8_t pcm = wave>>8;
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
        }
        if (!repeat) break;
    }
}
