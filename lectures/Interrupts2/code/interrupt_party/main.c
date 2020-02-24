#include "armtimer.h"
#include "console.h"
#include "gpio.h"
#include "gpioextra.h"
#include "interrupts.h"
#include "keyboard.h"
#include "printf.h"
#include "uart.h"
void console_cycle_color(void);
#define BUTTON GPIO_PIN21


static volatile int elapsed;

static bool button_pressed(unsigned int pc)
{
    if (gpio_check_and_clear_event(BUTTON)) {
        console_cycle_color();
        return true;
    }
    return false;
}

static bool second_elapsed(unsigned int pc) 
{
    if (armtimer_check_and_clear_interrupt()) {
        elapsed++;
        return true;
    }
    return false;
}

static bool clock_edge(unsigned int pc) 
{
    static int nClock;

    if (gpio_check_and_clear_event(KEYBOARD_CLOCK)) {
        nClock++;
        if (nClock % 11 == 0) uart_putchar('\a');
        return true;
    }
    return false;
}


void configure_button(void)
{
    gpio_set_input(BUTTON);
    gpio_set_pullup(BUTTON);
    gpio_enable_event_detection(BUTTON, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(button_pressed, INTERRUPTS_GPIO3);
}

void configure_alarm(void)
{
    // armtimer is intialized to number of usecs between events
    armtimer_init(1000000);
    armtimer_enable();             // enable timer itself
    armtimer_enable_interrupts();  // receive timer events as interrupts
    interrupts_attach_handler(second_elapsed, INTERRUPTS_BASIC_ARM_TIMER_IRQ);
}

void configure_keyboard(void)
{
    keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    gpio_enable_event_detection(KEYBOARD_CLOCK, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(clock_edge, INTERRUPTS_GPIO3);
}


void main(void) 
{
    gpio_init();
    uart_init();
    console_init(1, 9);
    interrupts_init();

    configure_button();
    configure_alarm();
    configure_keyboard();

    interrupts_global_enable();

    int last = 0;
    while (1) {
        if (last != elapsed) {
            console_clear();
            console_printf("  %02d:%02d", elapsed/60, elapsed % 60);
            last = elapsed;
        }
    }
}
