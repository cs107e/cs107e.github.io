#include "timer.h"
#include "gpio.h"

const int pin = GPIO_PIN20;

#define SECOND 1000

#define DOT (SECOND/6)
#define DASH (3*DOT)

#define SPACE DOT

#define LETTERSPACE (3*SPACE-SPACE)
#define WORD (7*DOT-LETTERSPACE)

void dot(void)
{
    gpio_write(pin, 1);
    timer_delay_ms(DOT);
    gpio_write(pin, 0);
    timer_delay_ms(SPACE);

}
void dash(void)
{
    gpio_write(pin, 1);
    timer_delay_ms(DASH);
    gpio_write(pin, 0);
    timer_delay_ms(SPACE);
}

void s(void)
{
   dot();
   dot();
   dot();
   timer_delay_ms(LETTERSPACE);
}

void o(void)
{
   dash();
   dash();
   dash();
   timer_delay_ms(LETTERSPACE);
}

void sos(void)
{
   s();
   o();
   s();
   timer_delay_ms(WORD);
}

void main(void)
{
    timer_init();
    gpio_init();

    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
    while (1)
        sos();
}
