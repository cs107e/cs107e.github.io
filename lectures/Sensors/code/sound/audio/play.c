#include "gpio.h"
#include "pwm.h"
#include "audio.h"
#include "sin8.h"

#define PHASE (3<<24)

void main ()
{
    gpio_init();
    pwm_init();
    audio_init();

    audio_write_u8(sinewave, PHASE);
}
