#include "armtimer.h"
#include "interrupts.h"
#include "pi.h"
#include "uart.h"

volatile int gCount = 0;

bool alarm(unsigned pc) 
{
    if (armtimer_check_and_clear_interrupt()) {
        gCount++;
        return true;
    }
    return false;
}

void main(void) 
{
    uart_init();
    interrupts_init();

    armtimer_init(1000000); // 1s
    armtimer_enable();
    armtimer_enable_interrupts();
    interrupts_register_handler(INTERRUPTS_BASIC_ARM_TIMER_IRQ, alarm);
    interrupts_enable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ);
    interrupts_global_enable(); 

	int nflips = 0;
	while (1) {
        while (nflips < gCount) {
            pi_led_toggle(PI_ACT_LED);
            nflips++;
        }
    }

    pi_reboot();
}
