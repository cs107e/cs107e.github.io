#include <stdint.h>
#include "printf.h"
#include "uart.h"
#include "timer.h"
void bug_compare(void) {
    int a = -20;
    unsigned int b = 6;

    if( a < b )
        printf("-20 < 6 - all is well\n");
    else
        printf("-20 >= 6 - omg \n");
}


unsigned int get_count() {
    static unsigned int count = 0xfffff000;
    return ++count;
}

void delay_ticks(unsigned int us) {
    unsigned int enough = us * 24;
    unsigned int start = get_count();
    unsigned int sofar = 0;
    unsigned int stop_at = start + enough;
    unsigned int now = 0;
    int count = 0;
    //while (get_count() - start < enough) {
    while (get_count() < start + enough) {
        count++;
        //now = get_count();
        //sofar = now - start;
        //if (get_count() - start >= enough) break;
        //if (now >= stop_at) break;
    }
    printf("iterated %d times, start was %02x, delta enough %d delta sofar %d stopat %x now %x\n", count, start, enough, sofar, stop_at, now);
}

void overflow(void) {
    for (unsigned char i = 0; i < 256; i++)
        printf("0x%2x\n", i);
}

void conversions(void) {
    uint16_t before = 0xffff;
    uint32_t after = before;

    // uint32_t before = 0x12340001;
    // uint16_t after = before;

    // int16_t before = -1;
    // int32_t after = before;

    // int32_t before = -50000;
    // int16_t after = before;

    // int32_t before = -1;
    // uint32_t after = before;

    printf("BEFORE: %d (0x%x) -> AFTER: %d (0x%x) \n", before, before, after, after);
}

void main(void) {
    uart_init();

  //  overflow();
  //  bug_compare();
   // conversions();
    for (int i = 0; i < 50; i++) {
        delay_ticks(5);
    }
}
