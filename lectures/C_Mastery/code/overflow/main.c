#include <stdint.h>
#include "printf.h"
#include "uart.h"
#include "timer.h"

static void bug_compare(void) {
    int a = -20;
    unsigned int b = 6;

    if (a < b)
        printf("-20 < 6 - all is well\n");
    else
        printf("-20 >= 6 - omg \n");
}


static unsigned int get_tick_count(void) {
    static unsigned int tc = 0xffffff00; // simulated timer at value close to overflow
    return ++tc;
}

static void wait_for(unsigned int delay) {
    unsigned int start = get_tick_count();
    printf("start %u + %d = stop %u ", start, delay, start + delay);
    int count = 0;

    while (get_tick_count() <= start + delay) { count++; }  // does NOT WORK if overflow
    //while (get_tick_count() - start <= delay) { count++; }  // does work even if overflow

    if (count == delay)
        printf("OK\n");
    else
        printf(" **** EARLY timeout, count = %d\n", count);
}

void main(void) {
    uart_init();

    bug_compare();
    wait_for(10);
    wait_for(300);
}
