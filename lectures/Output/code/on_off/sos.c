// S-O-S from Serial lecture
#include "timer.h"
#include "gpio.h"

const gpio_id_t pin = GPIO_PB4;

#define SECOND 1000

#define DOT (SECOND/6)
#define DASH (3*DOT)

#define SPACE DOT
#define LETTERSPACE (3*SPACE-SPACE)
#define WORDSPACE (7*SPACE-LETTERSPACE)

static void dot(void) {
    gpio_write(pin, 1);
    timer_delay_ms(DOT);
    gpio_write(pin, 0);
    timer_delay_ms(SPACE);
}

static void dash(void) {
    gpio_write(pin, 1);
    timer_delay_ms(DASH);
    gpio_write(pin, 0);
    timer_delay_ms(SPACE);
}

static void s(void) {
    dot();
    dot();
    dot();
    timer_delay_ms(LETTERSPACE);
}

static void o(void) {
    dash();
    dash();
    dash();
    timer_delay_ms(LETTERSPACE);
}

static void sos(void) {
    s();
    o();
    s();
    timer_delay_ms(WORDSPACE);
}

void main(void) {
    timer_init();
    gpio_init();

    gpio_set_output(pin);
    while (1) {
        sos();
    }
}
