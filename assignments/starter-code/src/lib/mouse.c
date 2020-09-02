#include "gpio.h"
#include "gpioextra.h"
#include "interrupts.h"
#include "ringbuffer.h"
#include "mouse.h"

#define CMD_RESET 0xFF
#define CMD_ENABLE_DATA_REPORTING 0xF4

static unsigned int CLK, DATA;
static rb_t *rb;

static void mouse_write(unsigned char data);
static bool clock_edge(unsigned int pc);

bool mouse_init(unsigned int clock_gpio, unsigned int data_gpio)
{
    CLK = clock_gpio;
    gpio_set_input(CLK);
    gpio_set_pullup(CLK);

    DATA = data_gpio;
    gpio_set_input(DATA);
    gpio_set_pullup(DATA);

    rb = rb_new();

    // FIXME: Initialize mouse.
    return true;
}

mouse_event_t mouse_read_event(void)
{
    mouse_event_t evt;

    // FIXME: Read scancode(s) and fill in evt

    return evt;
}

int mouse_read_scancode(void)
{
    // FIXME: Read from ring buffer
    return 0;
}

static void mouse_write(unsigned char data)
{
    // FIXME: Send host->mouse packet
}

static bool clock_edge(unsigned int pc)
{
    // FIXME: Process clock edge event
}
