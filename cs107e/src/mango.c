/*
 * Mango module
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Updated: Mon Jan  1 11:50:26 PST 2024
 */

#include "mango.h"
#include "timer.h"
#include "uart.h"

// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t irq_enable;
        uint32_t irq_status;
        uint32_t soft_reset;
        uint32_t reserved;
        uint32_t control;
        uint32_t config;
        uint32_t mode;
        uint32_t output;
    } regs;
} watchdog_t;

#define WATCHDOG_BASE ((watchdog_t *)0x20500A0)
_Static_assert(&(WATCHDOG_BASE->regs.mode)   == (uint32_t *)0x20500b8, "watchdog mode reg must be at address 0x20500b8");

static struct {
    volatile watchdog_t *wdog;
} const module = {
    .wdog = WATCHDOG_BASE
};

void mango_reboot(void) {
    timer_delay_ms(100); // give output time to flush (needed if using uart)
    const int cycles = 1;
    module.wdog->regs.config = 1; // config watchdog for whole system reset
    // mode = 0x16aa in upper 16 bits allows write to lower 15, cycles in bits [7:4], enable 1 in lsb [0]
    module.wdog->regs.mode = 0x16aa << 16 | (cycles << 4) | 1;
    while (1) ;
}

#define DOT_MS 120

static void on_dot(int ndot) {
    mango_actled(LED_ON);
    timer_delay_ms(ndot * DOT_MS);
}
static void off_dot(int ndot) {
    mango_actled(LED_OFF);
    timer_delay_ms(ndot * DOT_MS);
}

void mango_abort(void) {
    // Timing from https://en.wikipedia.org/wiki/SOS
    while (1) {  // S-O-S
        on_dot(1); off_dot(1); on_dot(1); off_dot(1); on_dot(1);
        off_dot(3);
        on_dot(3); off_dot(1); on_dot(3); off_dot(1); on_dot(3);
        off_dot(3);
        on_dot(1); off_dot(1); on_dot(1); off_dot(1); on_dot(1);
        off_dot(7);
    }
}

void mango_actled(enum led_state_t s) {
    static const gpio_id_t led = GPIO_PD18;

    gpio_set_output(led);
    if (s == LED_ON)
        gpio_write(led, 1);
    else if (s == LED_OFF)
        gpio_write(led, 0);
    else if (s == LED_TOGGLE)
        gpio_write(led, !gpio_read(led));
}
