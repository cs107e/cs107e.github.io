/*
 * Mango module
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Updated: Mon Jan  1 11:50:26 PST 2024
 */

#include "mango.h"
#include "timer.h"
#include "_system.h"
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
    if (sys_running_in_simulator()) {
        syscall_exit(0);  // divert iff in gdb sim
    }
    timer_delay_ms(100); // give output time to flush (needed if using uart)
    const int cycles = 1;
    module.wdog->regs.config = 1; // config watchdog for whole system reset
    // mode = 0x16aa in upper 16 bits allows write to lower 15, cycles in bits [7:4], enable 1 in lsb [0]
    module.wdog->regs.mode = 0x16aa << 16 | (cycles << 4) | 1;
    while (1) ;
}


void mango_abort(void) {
    if (sys_running_in_simulator()) {
        syscall_exit(1); // divert iff in gdb sim
    }
    uart_start_error(); // will force init uart if needed
    uart_putstring("\n *** In mango_abort(), type r to reboot: ");
    uart_end_error();
    while (1) {
        mango_actled(LED_TOGGLE);
        timer_delay_ms(100);
        if (uart_haschar()) {
            int ch = uart_recv();
            if (ch == 'r') {
                uart_putstring("rebooting\n");
                mango_reboot();
            }
        }
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
