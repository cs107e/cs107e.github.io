#include "gpio.h"
#include "pwm.h"
#include "tone.h"
#include "pitch.h"
#include "timer.h"

void main (void)
{
    gpio_init();
    gpio_set_function( 40, GPIO_FUNC_ALT0 ); // PWM0
    gpio_set_function( 45, GPIO_FUNC_ALT0 ); // PWM1
    gpio_set_function( 18, GPIO_FUNC_ALT5 );

    pwm_init();
    pwm_set_clock( F_AUDIO );

    //tone( NOTE_A4 );
    
    while( 1 ) {
        tone( NOTE_A4 );
        timer_delay_ms( 1000 );
        tone( NOTE_C4 );
        timer_delay_ms( 1000 );
    }
    while(1) ;
}
