/* File: gpioextra.c
 * -----------------
 * Support for GPIO event detection and internal pull-up/down
 * Julie Zelenski Sat Jan 27 09:15:07 PST 2018
 */

#include "gpio.h"
#include "gpioextra.h"
#include "_gpio_private.h"
#include <stddef.h>

/* 
Reminder from .h, this is the order the registers need to be placed in register array
enum gpio_event {
    GPIO_DETECT_RISING_EDGE = 0,
    GPIO_DETECT_FALLING_EDGE,
    GPIO_DETECT_HIGH_LEVEL,
    GPIO_DETECT_LOW_LEVEL,
    GPIO_DETECT_ASYNC_RISING_EDGE,
    GPIO_DETECT_ASYNC_FALLING_EDGE,
};
*/

static volatile uint32_t *gpio_reg_for_event(unsigned int event)
{
    volatile uint32_t *registers[] = { _gpio->ren, _gpio->fen, _gpio->hen, _gpio->len, _gpio->aren, _gpio->afen};

    if (event < 0 || event >= sizeof(registers)/sizeof(*registers)) return NULL;
    return registers[event];
}

static void gpio_set_detection(unsigned int pin, unsigned int event, unsigned int enabled)
{
    volatile uint32_t *reg = gpio_reg_for_event(event);

    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST || reg == NULL) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    if (enabled)
        reg[bank] |= 1 << shift;
    else
        reg[bank] &= ~(1 << shift);
    gpio_clear_event(pin);
}

bool gpio_get_event_detection(unsigned int pin, unsigned int event)
{
    volatile uint32_t *reg = gpio_reg_for_event(event);

    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST || reg == NULL) {
        return false;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    return (reg[bank] & (1 << shift)) != 0;
}

void gpio_disable_event_detection(unsigned int pin, unsigned int event)
{
    gpio_set_detection(pin, event, 0);
}

void gpio_enable_event_detection(unsigned int pin, unsigned int event)
{
    gpio_set_detection(pin, event, 1);
}

bool gpio_check_event(unsigned int pin) 
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return false;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    return (_gpio->eds[bank] & (1 << shift)) != 0;
}

void gpio_clear_event(unsigned int pin) 
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;
    if (gpio_check_event(pin))
        _gpio->eds[bank] = 1 << shift;
}

bool gpio_check_and_clear_event(unsigned int pin) 
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return false;
    }

    bool event = gpio_check_event(pin);
    if (event) gpio_clear_event(pin);
    return event;
}

void gpio_disable_all_event_detection(unsigned int pin) 
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }
    for (int i = GPIO_DETECT_RISING_EDGE; i <= GPIO_DETECT_ASYNC_FALLING_EDGE; i++)
        gpio_set_detection(pin, i, 0);
    gpio_clear_event(pin);
}

/*
 * From Broadcom manual about how to use the pull-up/down:
 * -------------------------------------------------------
 * The GPIO Pull-up/down Clock Registers control the actuation of internal
 * pull-downs on the respective GPIO pins. These registers must be used
 * in conjunction with the GPPUD register to effect GPIO Pull-up/down changes. 
 * The following sequence of events is required:
 * 1. Write to GPPUD to set the required control signal 
 *    (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down)
 * 2. Wait 150 cycles – this provides the required set-up time for the control signal
 * 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish 
 *    to modify – NOTE only the pads which receive a clock will be modified, 
 *    all others will retain their previous state.
 * 4. Wait 150 cycles – this provides the required hold time for the control signal
 * 5. Write to GPPUD to remove the control signal
 * 6. Write to GPPUDCLK0/1 to remove the clock
 */

enum { GPIO_PUD_DISABLE = 0, GPIO_PUD_PULLDOWN = 1, GPIO_PUD_PULLUP = 2 };

static void gpio_set_pud(unsigned int pin, unsigned int pud) 
{
    if (pin < GPIO_PIN_FIRST || pin > GPIO_PIN_LAST) {
        return;
    }

    unsigned int bank = pin / 32;
    unsigned int shift = pin % 32;

    _gpio->pud = pud;
    for (volatile int i = 0; i < 150; i++) ;

    _gpio->pudclk[bank] = 1 << shift;
    for (volatile int i = 0; i < 150; i++) ;

    _gpio->pudclk[bank] = 0;
}

void gpio_set_pullup(unsigned int pin) 
{
    gpio_set_pud(pin, GPIO_PUD_PULLUP);
}

void gpio_set_pulldown(unsigned int pin) 
{
    gpio_set_pud(pin, GPIO_PUD_PULLDOWN);
}

void gpio_set_pullnone(unsigned int pin) 
{
    gpio_set_pud(pin, GPIO_PUD_DISABLE);
}
