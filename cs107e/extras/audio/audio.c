/*
 * 				  AUDIO.C
 *  A file that allows a user to control audio output from the PWM of the pi.
 *  Has two modes -- SD / CM, the former is well-balanced PWM and the latter
 *  is blocked PWM.
 *
 *
 *
 *  @author aangelopouls hleou plevin pathanrahan
 *  @date 12 June 2017
*/
#include "gpio.h"
#include "timer.h"
#include "audio.h"
#include "mcp3008.h"
#include "printf.h"
#include "filters.h"
#include "pwm.h"
/*
 *  Sends a filtered signal out through the RPI PWM.
 *  Just put in a function that returns the next value to output.
 *
 *  @author aangelopoulos hleou
 *  @date 12 June 2017
*/
void audio_send_filter(unsigned (*function)(void)) {
    unsigned read = function();
    while(1) {
      int status =  pwm_get_status();
      if (!(status & PWM_FULL1)) {
	    pwm_write(read>>RIGHTSHIFT);
	    read = function();
      }
    }
}

void audio_send_waveform(unsigned * (*function)(unsigned length), unsigned samples) {
  unsigned * toPlay = function(samples);
  printf("Playing %d samples\n",samples);
  int i = 0;
  unsigned read = toPlay[i];
  while(i < samples) {
    int status = pwm_get_status();
    if(!(status & PWM_FULL1)) {
      pwm_write(read>>RIGHTSHIFT);
      read = toPlay[++i];
    }
  }
}

void audio_init() {
    gpio_set_function( GPIO_PIN40, GPIO_FUNC_ALT0 );
    gpio_set_function( GPIO_PIN45, GPIO_FUNC_ALT0 );
    delay_ms(DELAY);

    pwm_init();

    pwm_set_clock( 19200000/CLOCK_DIVISOR ); // 9600000 Hz
    delay_ms(DELAY);

    pwm_set_mode( 0, PWM_SIGMADELTA ); 
    pwm_set_mode( 1, PWM_SIGMADELTA );

    pwm_set_fifo( 0, 1 ); 
    pwm_set_fifo( 1, 1 );

    pwm_enable( 0 ); 
    pwm_enable( 1 );

    // pwm range is 1024 cycles
    pwm_set_range( 0, CYCLES );
    pwm_set_range( 1, CYCLES );
    delay_ms(2);
}
