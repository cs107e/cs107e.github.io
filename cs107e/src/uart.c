/*
 * Author: Pat Hanrahan <hanrahan@cs.stanford.edu>
 * Date: Jan 24, 2016
 */

#include "gpio.h"
#include <stdint.h>
#include "uart.h"

struct UART {
    uint32_t data; // I/O Data
    uint32_t ier;  // Interrupt enable
    uint32_t iir;  // Interrupt identify and fifo enables/clears
    uint32_t lcr;  // line control register
    uint32_t mcr;  // modem control register
    uint32_t lsr;  // line status register
    uint32_t msr;  // modem status register
    uint32_t scratch;
    uint32_t cntl; // control register
    uint32_t stat; // status register
    uint32_t baud; // baud rate register
} ;

// AUX bits
#define AUX_ENABLES 0x20215004
#define AUX_ENABLE  0x00000001

// Mini UART
#define MINI_UART_BASE 0x20215040

#define MINI_UART_IIR_RX_FIFO_CLEAR  0x00000002
#define MINI_UART_IIR_TX_FIFO_CLEAR  0x00000004
#define MINI_UART_IIR_RX_FIFO_ENABLE 0x00000080
#define MINI_UART_IIR_TX_FIFO_ENABLE 0x00000040

#define MINI_UART_LCR_8BIT       0x00000003

#define MINI_UART_LSR_RX_READY   0x00000001
#define MINI_UART_LSR_TX_READY   0x00000010
#define MINI_UART_LSR_TX_EMPTY   0x00000020

#define MINI_UART_CNTL_TX_ENABLE 0x00000002
#define MINI_UART_CNTL_RX_ENABLE 0x00000001

static volatile struct UART *uart = (struct UART*) MINI_UART_BASE;


/* Key detail from the Broadcom Peripherals data sheet p.10
 *
 * GPIO pins should be set up first the before enabling the UART. 
 * The UART core is build to emulate 16550 behaviour. 
 * So when it is enabled any data at the inputs will immediately be received .
 * If the UART1_RX line is low (because the GPIO pins have not been set-up yet) 
 * that will be seen as a start bit and the UART will start receiving 0x00-characters.
 * [...] The result will be that the FIFO is full and overflowing in no time flat.
 */

void uart_init(void) 
{
    gpio_set_function(GPIO_TX, GPIO_FUNC_ALT5);
    gpio_set_function(GPIO_RX, GPIO_FUNC_ALT5);

    int *aux = (int*)AUX_ENABLES;
    *aux |= AUX_ENABLE; // must enable mini-uart before accessing registers

    uart->ier  = 0;
    uart->cntl = 0;
    uart->lcr  = MINI_UART_LCR_8BIT;
    uart->mcr  = 0;
    uart->ier  = 0;
    uart->iir  = MINI_UART_IIR_RX_FIFO_CLEAR |
                 MINI_UART_IIR_RX_FIFO_ENABLE |
                 MINI_UART_IIR_TX_FIFO_CLEAR |
                 MINI_UART_IIR_TX_FIFO_ENABLE;

    uart->baud = 270; // baud rate ((250,000,000/115200)/8)-1 = 270

    uart->cntl = MINI_UART_CNTL_TX_ENABLE | MINI_UART_CNTL_RX_ENABLE;
}

// RE: line endings
// canonial use is '\n' newline as sole line terminator (both read/write)
// but connected terminal may expect to receive a CR-LF sequence from Pi
// and may send a CR to Pi for return/enter key. art_getchar and uart_putchar
// take care of converting internally so that client can simply 
// send/receive \n

int uart_getchar(void) {
    while (!(uart->lsr & MINI_UART_LSR_RX_READY)) ;
    int ch = uart->data & 0xFF;
    if (ch == EOT) return EOF;      // convert EOT to EOF
    if (ch == '\r') return '\n';    // convert CR to newline
    return ch;
}

int uart_putchar(int ch) {
    // convert newline to CR LF sequence by inserting CR
    if (ch == '\n') {
        uart_putchar('\r');
    }
    while (!(uart->lsr & MINI_UART_LSR_TX_EMPTY)) ;
    uart->data = ch;
    return ch;
}

void uart_flush(void) {
    while (!(uart->lsr & MINI_UART_LSR_TX_EMPTY)) ;
}

bool uart_haschar(void) {
    return (uart->lsr & MINI_UART_LSR_RX_READY);
}

int uart_putstring(const char *str) {
    int n = 0;
    while (str[n]) {
        uart_putchar(str[n]);
        n++;
    }
    return n;
}
