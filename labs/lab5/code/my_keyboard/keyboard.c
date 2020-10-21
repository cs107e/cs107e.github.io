#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "ps2.h"

static unsigned int CLK, DATA;

void keyboard_init(unsigned int clock_gpio, unsigned int data_gpio) 
{
    CLK = clock_gpio;
    gpio_set_input(CLK);
    gpio_set_pullup(CLK); 
 
    DATA = data_gpio;
    gpio_set_input(DATA);
    gpio_set_pullup(DATA); 
}

static int read_bit(void) 
{
    // falling edge on clock triggers read 
    // wait until clock reads high, then wait until clock reads low
    // now read data
    return -1; // TODO
}

unsigned char keyboard_read_scancode(void) 
{
    // TODO: Implement this function during lab5!
    return 0xff;
}

key_action_t keyboard_read_sequence(void)
{
    // The implementation started below assumes sequence is exactly
    // one byte long. This is the case for ordinary key press actions
    // but not same for all key actions.
    // What key actions generate a sequence of length 2?  What
    // about length 3?
    // Figure out what those cases are and extend this code to
    // properly handle those sequences and generate the correct
    // key actions
    key_action_t action;
    action.keycode = keyboard_read_scancode();
    action.what = KEY_PRESS;
    return action;
}
