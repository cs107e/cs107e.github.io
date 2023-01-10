#pragma once
#include "pi.h"
void ref_pi_reboot(void) __attribute__ ((noreturn));
void ref_pi_abort(void) __attribute__ ((noreturn));
void ref_pi_led_on(int led);
void ref_pi_led_off(int led);
void ref_pi_led_toggle(int led);
