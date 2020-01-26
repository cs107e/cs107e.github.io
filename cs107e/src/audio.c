#include <stdint.h>
#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "audio.h"

/*
   Initialize the pwm for audio.

   - base clock is 9600000
   - pwm range is 256  for a sample clock rate of 37500 Khz
   - writing a 256 sample waveform will repeat sound at 37500/256 = 146.5 Hz
*/
void audio_init(void) 
{
    gpio_set_function( 40, GPIO_FUNC_ALT0 );
    gpio_set_function( 45, GPIO_FUNC_ALT0 );
    timer_delay_ms(2);

    pwm_init();

    pwm_set_clock( 19200000/2 ); // 9600000 Hz
    timer_delay_ms(2);

    pwm_set_mode( 0, PWM_SIGMADELTA ); 
    pwm_set_mode( 1, PWM_SIGMADELTA );

    pwm_set_fifo( 0, 1 ); 
    pwm_set_fifo( 1, 1 );

    pwm_enable( 0 ); 
    pwm_enable( 1 );

    // pwm range is 256 cycles
    pwm_set_range( 0, 0x100 );
    pwm_set_range( 1, 0x100 );
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
   
   These functions do not return.
*/

void audio_write_u8(const uint8_t waveform[], unsigned dphase) 
{
    unsigned phase = 0;
    while(1) {
        unsigned status = pwm_get_status();
        if (!(status & PWM_FULL1)) {
            unsigned angle = phase >> 24;
            uint8_t pcm = waveform[angle];
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
            phase += dphase; 
        }
    }
}

void audio_write_u16(const uint16_t waveform[], unsigned dphase) 
{
    unsigned phase = 0;
    while(1) {
        unsigned status = pwm_get_status();
        if (!(status & PWM_FULL1)) {
            unsigned angle = phase >> 24;
            uint8_t pcm = waveform[angle] >> 8;
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
            phase += dphase; 
        }
    }
}

void audio_write_i16(const int16_t waveform[], unsigned dphase) 
{
    unsigned phase = 0;
    while(1) {
        unsigned status = pwm_get_status();
        if (!(status & PWM_FULL1)) {
            unsigned angle = phase >> 24;
            unsigned wave = waveform[angle] - INT16_MIN;
            uint8_t pcm = wave>>8;
            pwm_write( pcm ); // output to channel 0
            pwm_write( pcm ); // output to channel 1
            phase += dphase; 
        }
    }
}
