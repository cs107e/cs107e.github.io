#include "timer.h"
#include "gpio.h"
#include "pwm.h"
#include "tone.h"
#include "pitch.h"

// notes in the melody:
int melody[] = { 
    NOTE_C4, 
    NOTE_G3,
    NOTE_G3,
    NOTE_A3,
    NOTE_G3,
    0, 
    NOTE_B3,
    NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void main ( void )
{
    timer_init();

    gpio_init();
    gpio_set_function( 40, GPIO_FUNC_ALT0 ); // PWM0
    gpio_set_function( 45, GPIO_FUNC_ALT0 ); // PWM1
    gpio_set_function( 18, GPIO_FUNC_ALT5 ); // PWM0

    pwm_init();
    pwm_set_clock( F_AUDIO );

    while( 1 ) {
        int thisNote;
        for (thisNote = 0; thisNote < 8; thisNote++) {
            // to calculate the note duration, take one second
            // divided by the note type.
            //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            int noteDuration = 1000/noteDurations[thisNote];
            tone(melody[thisNote]);
            timer_delay_ms(noteDuration);

            // to distinguish the notes, set a minimum time between them.
            // the note's duration + 30% seems to work well:
            int pauseBetweenNotes = noteDuration * 0.30;
            tone(0); // 0 turns off sound
            timer_delay_ms(pauseBetweenNotes);
        }
    }
}
