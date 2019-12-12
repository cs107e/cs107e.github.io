/* File: pi.c
 * ----------
 * Utility functions for Raspberry Pi.
 * (reboot/abort execution, control on-board LEDs)
 * Author: Pat Hanrahan Mar 1, 2016
 * Author: Julie Zelenski Sat Jan 27 09:15:07 PST 2018
 */

#include "pi.h"
#include <stdint.h>
#include "timer.h"

void pi_reboot(void) 
{
    static const int PM_RSTC = 0x2010001c;
    static const int PM_WDOG = 0x20100024;
    static const int PM_PASSWORD = 0x5a000000;
    static const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;

    volatile uint32_t * reg_pm_wdog = (uint32_t *)PM_WDOG;
    volatile uint32_t * reg_pm_rstc = (uint32_t *)PM_RSTC;

    // give output time to flush.
    for (int i = 0; i < 100000; i++) {
        __asm__("nop");
    }

    // timeout = 1/16th of a second? (whatever)
    *reg_pm_wdog = PM_PASSWORD | 1;
    *reg_pm_rstc = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;
    while (1)
        ;
}

void pi_abort(void) 
{
    gpio_set_output(PI_PWR_LED);
    while (1) {  // Infinite loop, flash the red PWR LED
        pi_led_toggle(PI_PWR_LED);
        timer_delay_ms(200);
    }
}

void pi_led_on(int led) 
{
    if (led != PI_ACT_LED && led != PI_PWR_LED) return;
    gpio_set_output(led);
    gpio_write(led, 1);
}

void pi_led_off(int led) 
{
    if (led != PI_ACT_LED && led != PI_PWR_LED) return;
    gpio_set_output(led);
    gpio_write(led, 0);
}

void pi_led_toggle(int led)
{
    if (led != PI_ACT_LED && led != PI_PWR_LED) return;
    gpio_set_output(led);
    gpio_write(led, !gpio_read(led));
}
