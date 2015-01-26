#include "gpio.h"
#include "timer.h"
#include "uart.h"

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
#define TX 14
#define RX 15

struct UART {
    int data; // I/O Data
    int ier;  // Interupt enable
    int iir;  // Interupt identify and fifo enables/clears
    int lcr;  // line control register
    int mcr;  // modem control register
    int lsr;  // line status
    int msr;  // line status
    int scratch;
    int cntl; // control register
    int stat; // status register
    int baud; // baud rate register
} ;

// AUX bits
#define AUX_ENABLES     0x20215004
#define AUX_MU_ENABLE   0x00000001
#define AUX_SPI0_ENABLE 0x00000002
#define AUX_SPI1_ENABLE 0x00000004

// Mini UART bits
#define AUX_MU_UART     0x20215040

#define AUX_MU_IIR_RX_FIFO_CLEAR  0x00000002
#define AUX_MU_IIR_TX_FIFO_CLEAR  0x00000004
#define AUX_MU_IIR_RX_FIFO_ENABLE 0x00000008
#define AUX_MU_IIR_TX_FIFO_ENABLE 0x00000004

#define AUX_MU_LCR_8BIT       0x00000003

#define AUX_MU_LSR_RX_READY   0x00000001
#define AUX_MU_LSR_TX_READY   0x00000010
#define AUX_MU_LSR_TX_EMPTY   0x00000020

#define AUX_MU_CNTL_TX_ENABLE 0x00000002
#define AUX_MU_CNTL_RX_ENABLE 0x00000001

volatile struct UART *uart = (struct UART*) AUX_MU_UART;

void uart_init ( void )
{
    int *aux = (int*)AUX_ENABLES;;

    *aux = AUX_MU_ENABLE; // turn on mini-uart

    uart->ier  = 0;
    uart->cntl = 0;
    uart->lcr  = AUX_MU_LCR_8BIT; 
    uart->mcr  = 0;
    uart->ier  = 0;
    uart->iir  = AUX_MU_IIR_RX_FIFO_CLEAR|
                 AUX_MU_IIR_RX_FIFO_ENABLE|
                 AUX_MU_IIR_TX_FIFO_CLEAR|
                 AUX_MU_IIR_TX_FIFO_ENABLE; 
    uart->baud =  270; // baud rate ((250,000,000/115200)/8)-1 = 270

    pinMode( TX, ALT5 ); 
    pinMode( RX, ALT5 ); 

    //pinPullUpDown( TX, 0 ); // disable pull ups/downs on TX
    //pinPullUpDown( RX, 0 ); // disable pull ups/downs on RX

    uart->cntl = AUX_MU_CNTL_TX_ENABLE|AUX_MU_CNTL_RX_ENABLE;; 
}

int uart_getc ( void )
{
    while(1)
        if( uart->lsr & AUX_MU_LSR_RX_READY ) break;
    return uart->data & 0xFF;
}

void uart_putc ( unsigned int c )
{
    if(c=='\n') uart_putc('\r');
    while(1)
         if( uart->lsr & AUX_MU_LSR_TX_EMPTY ) break;
    uart->data = c;
}

void uart_flush ( void )
{
    while(1)
        if( uart->lsr & AUX_MU_LSR_TX_EMPTY ) break;
}
