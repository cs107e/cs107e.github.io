#ifndef GPIOEXTRA_REF
#define GPIOEXTRA_REF
#include "gpioextra.h"
void ref_gpio_enable_event_detection(unsigned int pin, unsigned int event);
void ref_gpio_disable_event_detection(unsigned int pin, unsigned int event);
bool ref_gpio_get_event_detection(unsigned int pin, unsigned int event);
void ref_gpio_disable_all_event_detection(unsigned int pin);
bool ref_gpio_check_event(unsigned int pin);
void ref_gpio_clear_event(unsigned int pin);
bool ref_gpio_check_and_clear_event(unsigned int pin);
void ref_gpio_set_pullup(unsigned int pin);
void ref_gpio_set_pulldown(unsigned int pin);
void ref_gpio_set_pullnone(unsigned int pin);
#endif
