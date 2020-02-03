/* Bootloader for CS107E Winter 2018.
 * Author: Julie Zelenski, Jan 2018
 * Based on original bootloader03 by dwelch + Dawson rewrites
 */
#include "pi.h"
#include "timer.h"
#include "uart.h"

enum { 
    SOH = 0x01,
    ACK = 0x06,
    NAK = 0x15,
//    EOT = 0x04,    // defined in uart
    PAYLOAD_SIZE = 128,
    TIMEOUT = -1,
    ARMBASE = 0x8000
};

void BRANCHTO ( unsigned int );

static void flash_heartbeat(void)
{
    for (int i = 0; i < 4; i++) {
        pi_led_toggle(PI_ACT_LED);
        timer_delay_ms(75);
    }
}

static void wait_for_transmit(void)
{
    while (1) {
        timer_delay_ms(500);
        if (uart_haschar()) return;
        flash_heartbeat();
        uart_putchar(NAK);    
    }
}

#define HALF_SEC 50000

static unsigned int wait_for_byte(void)
{
    unsigned t0 = timer_get_ticks();
    while (!uart_haschar()) {
        if ((timer_get_ticks() - t0) >= HALF_SEC) 
            return TIMEOUT;
    }
    return uart_getchar();
}

static void receive_program(void) 
{
    unsigned char seqnum = 1;
    unsigned char *addr = (unsigned char *)ARMBASE;
    int nerrors = 0;

    pi_led_on(PI_ACT_LED);
    while (nerrors < 5) {
        unsigned char b = wait_for_byte();
        
        if (b == EOT) {
            pi_led_off(PI_ACT_LED);
            uart_putchar(ACK);
            timer_delay_ms(500);
            BRANCHTO(ARMBASE);
            return; // NOTREACHED
        }
        if (b != SOH 
              || wait_for_byte() != seqnum 
              || wait_for_byte() != (0xFF - seqnum)) {
            uart_putchar(NAK);
            ++nerrors;
            continue;  
        }
        unsigned char cksum = 0;
        for (int i = 0; i < PAYLOAD_SIZE; i++) {
            if ((b = wait_for_byte()) == TIMEOUT) return;
            cksum += b;
            *(addr+i) = b;
        }
        if (wait_for_byte() != cksum) {
            uart_putchar(NAK);
            ++nerrors;
        } else {
            uart_putchar(ACK);
            addr += PAYLOAD_SIZE;
            ++seqnum;
            nerrors = 0;
        }
    }
}

void bootloader_main(void) 
{
    uart_init();
    while (1) {
        wait_for_transmit();
        receive_program();
    }
}	