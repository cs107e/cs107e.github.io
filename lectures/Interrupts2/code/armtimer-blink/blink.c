#include "pi.h"
#include "armtimer.h"
#include "uart.h"
#include "interrupts.h"
#include "printf.h"

#define ONE_SEC 1000000

static volatile int gcount = 0;   // why must this be volatile?

static bool timer_event(unsigned int pc) 
{
    if (armtimer_check_and_clear_interrupt()) {
        gcount++;
        return true;
    }
    return false;
}

void main(void) 
{
	uart_init();

    // init argument is number of usecs between events
	armtimer_init(ONE_SEC);
    armtimer_enable();      // enable timer module

    // enable timer events as interrupts
    armtimer_enable_interrupts();

    // enable timer as interrupt source
    interrupts_enable_basic(INTERRUPTS_BASIC_ARM_TIMER_IRQ);

    // attach interrupt handler
    interrupts_attach_handler(timer_event);

    // enable interrupts globally
    interrupts_global_enable(); 

	int last = 0;
    printf("Will toggle ACT led every second...\n");
	while (1) {
		if (last != gcount) {
			last = gcount;
            pi_led_toggle(PI_ACT_LED);
			printf("received %d interrupts\n", last);
		}
	}
}
