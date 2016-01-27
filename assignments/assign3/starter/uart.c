#include "bcm.h"
#include "gpio.h"
#include "uart.h"

struct UART {
    int data; // I/O Data
    int ier;  // Interupt enable
    int iir;  // Interupt identify and fifo enables/clears
    int lcr;  // line control register
    int mcr;  // modem control register
    int lsr;  // line status register
    int msr;  // modem status register
    int scratch;
    int cntl; // control register
    int stat; // status register
    int baud; // baud rate register
} ;

// AUX bits
#define AUX_ENABLES 0x20215004
#define AUX_ENABLE  0x00000001

// Mini UART 
#define MINI_UART_BASE 0x20215040

#define MINI_UART_IIR_RX_FIFO_CLEAR  0x00000002
#define MINI_UART_IIR_TX_FIFO_CLEAR  0x00000004
#define MINI_UART_IIR_RX_FIFO_ENABLE 0x00000008
#define MINI_UART_IIR_TX_FIFO_ENABLE 0x00000004

#define MINI_UART_LCR_8BIT       0x00000003

#define MINI_UART_LSR_RX_READY   0x00000001
#define MINI_UART_LSR_TX_READY   0x00000010
#define MINI_UART_LSR_TX_EMPTY   0x00000020

#define MINI_UART_CNTL_TX_ENABLE 0x00000002
#define MINI_UART_CNTL_RX_ENABLE 0x00000001

static volatile struct UART *uart = (struct UART*) MINI_UART_BASE;

void uart_init ( void )
{
    int *aux = (int*)AUX_ENABLES;;

    *aux = AUX_ENABLE; // turn on mini-uart

    uart->ier  = 0;
    uart->cntl = 0;
    uart->lcr  = MINI_UART_LCR_8BIT;
    uart->mcr  = 0;
    uart->ier  = 0;
    uart->iir  = MINI_UART_IIR_RX_FIFO_CLEAR |
                 MINI_UART_IIR_RX_FIFO_ENABLE |
                 MINI_UART_IIR_TX_FIFO_CLEAR |
                 MINI_UART_IIR_TX_FIFO_ENABLE;

    uart->baud =  270; // baud rate ((250,000,000/115200)/8)-1 = 270

    gpio_set_function( GPIO_TX, GPIO_FUNC_ALT5 );
    gpio_set_function( GPIO_RX, GPIO_FUNC_ALT5 );

    uart->cntl = MINI_UART_CNTL_TX_ENABLE | MINI_UART_CNTL_RX_ENABLE;;
}

int uart_getc ( void )
{
    while ( !(uart->lsr & MINI_UART_LSR_RX_READY) ) ;
    return uart->data & 0xFF;
}

void uart_putc ( unsigned c )
{
    if (c == '\n') uart_putc('\r');
    while ( !(uart->lsr & MINI_UART_LSR_TX_EMPTY) ) ;
    uart->data = c;
}

void uart_flush ( void )
{
    while ( !(uart->lsr & MINI_UART_LSR_TX_EMPTY) ) ;
}
