/* 
 * Bootloader for CS107E
 * Author: Julie Zelenski, updated Mar 2020
 * Based on original bootloader03 by dwelch + Dawson rewrites
 */

#include "pi.h"
#include "timer.h"
#include "uart.h"

enum { 
    SOH = 0x01,     // Start of heading
    ACK = 0x06,     // Acknowledge (positive)
    NAK = 0x15,     // Acknowledge (negative)
    PAYLOAD_SIZE = 128,
    TIMEOUT = -1,
};

static void * const ARMBASE = (void *)0x8000;
void BRANCHTO ( void * );

static void flash_heartbeat(void)
{
    for (int i = 0; i < 4; i++) {
        pi_led_toggle(PI_ACT_LED);
        timer_delay_ms(75);
    }
}

static int receive_byte(void)
{
    unsigned t0 = timer_get_ticks();

    while (1) {
        if (uart_haschar()) 
            return uart_recv();
        if ((timer_get_ticks() - t0) >= 1000000) // one sec
            return TIMEOUT;
    }
}

static bool receive_packet(unsigned char *dst, int expected_seq)
{
    unsigned char seq = receive_byte();
    unsigned char seq_inv = receive_byte();
    unsigned char cksum = 0;
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        int b = receive_byte();
        if (b == TIMEOUT) return false;
        *(dst + i) = b;
        cksum += b;
    }
    unsigned char last = receive_byte();
    return ((seq == expected_seq) && (seq_inv == (0xFF - seq)) && (last == cksum));
}

static void receive_loop(void) 
{
    unsigned char seqnum = 1;
    unsigned char *addr = ARMBASE;

    while (1) {
        int b = receive_byte();

        if (b == TIMEOUT) {
            flash_heartbeat();
            uart_send(NAK);
            addr = ARMBASE;
            seqnum = 1;
        } else if (b == SOH) {
            bool success = receive_packet(addr, seqnum);
            if (success) {
                uart_send(ACK);
                addr += PAYLOAD_SIZE;
                seqnum++;
           } else {
                uart_send(NAK);
            }
        } else if (b == EOT) {
            uart_putchar(ACK);
            timer_delay_ms(500);
            BRANCHTO(ARMBASE);
            return; // NOTREACHED
        }
    }
}

void bootloader_main(void) 
{
    uart_init();
    receive_loop();
}   