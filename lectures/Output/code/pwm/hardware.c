/* File: hardware.c
 * ----------------
 * Offloads work to hardware pwm peripheral
 *
 * Set color components of common anode RGB led
 * Square wave audio output to passive buzer
 */
#include "gpio.h"
#include "gl.h"
#include "pwm.h"
#include "rand.h"
#include "timer.h"

enum {
    NOTE_C3 = 131,
    NOTE_D3 = 147,
    NOTE_E3 = 165,
    NOTE_F3 = 175,
    NOTE_G3 = 196,
    NOTE_A3 = 220,
    NOTE_B3 = 247,
    NOTE_C4 = 262,
};

static const struct {
    gpio_id_t pin;
    pwm_channel_id_t ch;
}   RED =      { GPIO_PB10, PWM7 },
    GREEN  =   { GPIO_PB11, PWM2 },
    BLUE =     { GPIO_PB12, PWM0 },
    AUDIO =    { GPIO_PB0,  PWM3 };

static void led_color(color_t c) {
    uint8_t b = c & 0xff;
    uint8_t g = (c >> 8) & 0xff;
    uint8_t r = (c >> 16) & 0xff;
    const uint8_t max = 255;
    pwm_set_duty(RED.ch, r*100/max);
    pwm_set_duty(GREEN.ch, g*100/max);
    pwm_set_duty(BLUE.ch, b*100/max);
}

static void play_tone(int freq) {
    pwm_set_freq(AUDIO.ch, freq);
}

static void play_scale(void) {
    const int c_major_scale[] = { NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4 };
    const color_t rainbow[] =   { GL_RED, GL_ORANGE, GL_YELLOW, GL_GREEN, GL_BLUE, GL_INDIGO, GL_PURPLE, GL_WHITE };
    int octave = sizeof(c_major_scale)/sizeof(*c_major_scale);

    for (int i = 0; i < octave; i++) {
        led_color(rainbow[i]);
        play_tone(c_major_scale[i]);
        timer_delay_ms(300);
    }
    timer_delay(1);
    play_tone(0);
    led_color(GL_BLACK);
}

static void sound_effects(void) {
    int low_freq = 274, high_freq = 1093;
    int n = 50;
    for (int i = 0; i < n; i++) {
        int f = low_freq;
        int delta = 1 + n*2 - 2*i;
        play_tone(f);
        timer_delay_ms(20);
        while (f < high_freq) {
            f += delta;
            led_color(rand());
            play_tone(f);
            timer_delay_us((1000*1000/f));
        }
    }
    play_tone(0);
}

void main(void) {
    pwm_init();
    pwm_config_channel(BLUE.ch, BLUE.pin, 75, true);
    pwm_config_channel(RED.ch, RED.pin, 75, true);
    pwm_config_channel(GREEN.ch, GREEN.pin, 75, true);
    pwm_config_channel(AUDIO.ch, AUDIO.pin, 1200000, false);
    play_scale();
    timer_delay(1);
    sound_effects();
}
