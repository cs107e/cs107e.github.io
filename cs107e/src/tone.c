#include "pwm.h"
#include "tone.h"

/*
 * this version is fragile. because divisor is 12-bits
 * we need to change the pwm clock depending on the input
 * frequency. 
 */
void tone(int freq) {
    if (freq == 0) {
        pwm_disable(0);
        pwm_disable(1);
        return;
    }
    pwm_set_mode(0, PWM_MARKSPACE); // PWM0, MARKSPACE
    pwm_set_mode(1, PWM_MARKSPACE); // PWM1, MARKSPACE

    pwm_enable(0);
    pwm_enable(1);

    // assumes pwm_clock(F_AUDIO)
    int divisor = F_AUDIO / freq;
    int divisor2 = divisor / 2;

    pwm_set_range(0,  divisor);
    pwm_set_width(0,  divisor2);

    pwm_set_range(1,  divisor);
    pwm_set_width(1,  divisor2);

}
