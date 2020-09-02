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


unsigned char keyboard_read_scancode(void) 
{
    // TODO: Your code here
    return 0x7e;
}

key_action_t keyboard_read_sequence(void)
{
    // TODO: Your code here
    key_action_t action;
    return action;
}

key_event_t keyboard_read_event(void) 
{
    // TODO: Your code here
    key_event_t event;
    return event;
}


unsigned char keyboard_read_next(void) 
{
    // TODO: Your code here
    return '!';
}
