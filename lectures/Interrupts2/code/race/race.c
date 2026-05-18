#include "hstimer.h"
#include "interrupts.h"
#include "printf.h"
#include "rand.h"
#include "timer.h"
#include "uart.h"

// written by both, unprotected
static volatile int unsafe_count = 0;

//written by both, with protection
static volatile int safe_count = 0;

// written only by one or the other
static volatile int n_interrupt = 0, n_main = 0;

static void timer_event(void *unused) {
    hstimer_interrupt_clear(HSTIMER0);
    unsafe_count++; // written by both interrupt and mainline
    safe_count++;
    n_interrupt++;
}

void main(void) {
    uart_init();
    printf("Starting %s\n", __FILE__);
    interrupts_init();

    hstimer_init(HSTIMER0, 20, HSTIMER_PERIODIC);
    hstimer_set_handler(HSTIMER0, timer_event, NULL);
    hstimer_enable(HSTIMER0);

    interrupts_global_enable();

    for (int i = 0; i < 100000; i++) {
        timer_delay_us(rand() % 100);

        unsafe_count--; // written by both interrupt and mainline

        interrupts_global_disable();  // enter critical section
        safe_count--;   // written by both, but within critical section so protected
        interrupts_global_enable();    // exit critical section

        n_main++;  // only written by mainline

        if (i && (i % 10000 == 0)) {  // report every 10,0000
            printf("@%d: %6d\n", i, safe_count - unsafe_count);
        }
    }
    hstimer_disable(HSTIMER0); // turn off timer

    printf("\nMainline--: %d, Interrupt++: %d\n", n_main,  n_interrupt);
    printf("Count expected: %d should == safe_count %d\n", n_interrupt - n_main, safe_count);
    printf("Unsafe_count: %d\n", unsafe_count);
    printf("\n");
    int lost = safe_count - unsafe_count;
    printf("Num updates lost: %d of %d (%.2f%%)\n", lost, n_main, (lost*100.0)/n_main);
}
