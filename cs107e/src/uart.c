/*
 * Hardware abstractions for a serial port (UART).
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Last updated: Wed Dec 27 16:28:18 PST 2023
 */

#include "uart.h"
#include <stddef.h>
#include "assert.h"
#include "ccu.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "interrupts.h"

#define LCR_DLAB            (1 << 7)
#define USR_BUSY            (1 << 0)
#define USR_TX_NOT_FULL     (1 << 1)
#define USR_TX_NOT_EMPTY    (1 << 2)
#define USR_RX_NOT_EMPTY    (1 << 3)

/*
 * D1 uart has 6 UART controllers (UART0, UART1, UART2, UART3, UART4, UART5)
 * Compatible with industry-standard 16450/16550
 * peripheral registers similar to rpi
 * TX/RX fifo, can be interrupt-driven
 */

// structs defined to match layout of hardware registers
typedef union {
    struct {
        union {
            uint32_t rbr;   // receive buffer register
            uint32_t thr;   // transmit holding register
            uint32_t dll;   // divisor latch (LSB)
        };
        union {
            uint32_t dlh;   // divisor latch (MSB)
            uint32_t ier;   // interrupt enable register
        };
        union {
            uint32_t iir;   // interrupt identification register
            uint32_t fcr;   // FIFO control register
        };
        uint32_t lcr;       // line control register
        uint32_t mcr;       // modem control register
        uint32_t lsr;       // line status register
        uint32_t reserved[25];
        uint32_t usr;       // busy status, at offset 0x7c
        uint32_t reserved2[9];
        uint32_t halt;      // at offset 0xa4
    } regs;
    unsigned char padding[0x400];
} uart_t;

#define UART_BASE ((uart_t *)0x02500000)
_Static_assert(&(UART_BASE[0].regs.lcr) == (uint32_t *)0x0250000C, "UART0 lcr reg must be at address 0x0250000C");
_Static_assert(&(UART_BASE[1].regs.dlh) == (uint32_t *)0x02500404, "UART1 dlh reg must be at address 0x02500404");

typedef struct {
    int index;
    gpio_id_t tx, rx;
    unsigned int fn;
} uart_config_t;

static struct {
    volatile uart_t *uart_base, *uart;
    uart_config_t config;
} module = { .uart_base = UART_BASE,
             .uart = NULL, // will be set in uart_init
};

// not published for now, used to do testing of alternate uarts
void uart_reinit_custom(int, gpio_id_t, gpio_id_t, unsigned int);

void uart_reinit_custom(int uart_id, gpio_id_t tx, gpio_id_t rx, unsigned int gpio_fn) {
    if (module.uart) {  // shut down previous if active
        uart_flush();
        gpio_set_function(module.config.tx, GPIO_FN_DISABLED); // disconnect gpio
        gpio_set_pullnone(module.config.tx);
        gpio_set_function(module.config.rx, GPIO_FN_DISABLED);
        gpio_set_pullnone(module.config.rx);
        module.uart = NULL;
    }

    module.config.index = uart_id;
    module.config.tx = tx;
    module.config.rx = rx;
    module.config.fn = gpio_fn;
    module.uart = module.uart_base + module.config.index;

    // clock up peripheral
    // gating bits [0:5], reset bits [16:21]
    uint32_t bit = 1 << module.config.index;
    uint32_t reset = bit << 16;
    ccu_enable_bus_clk(CCU_UART_BGR_REG, bit, reset);

    // configure GPIOs
    gpio_set_function(module.config.tx, module.config.fn);
    gpio_set_pullup(module.config.tx);
    gpio_set_function(module.config.rx, module.config.fn);
    gpio_set_pullup(module.config.rx);

    // configure baud rate
    uint32_t baud = 115200;
    module.uart->regs.fcr = 1;      // enable TX/RX fifo
    module.uart->regs.halt = 1;     // temporarily disable TX transfer

    uint32_t sys_clock_rate = 24 * 1000000;
    uint32_t udiv = sys_clock_rate / (16 * baud);
    module.uart->regs.lcr |= LCR_DLAB;  // set DLAB = 1 to access DLL/DLH
    module.uart->regs.dll = udiv & 0xff;        // low byte of divisor -> DLL
    module.uart->regs.dlh = (udiv >> 8) & 0xff; // hi byte of divisor -> DLH
    module.uart->regs.lcr &= ~LCR_DLAB; // set DLAB = 0 to access RBR/THR
    module.uart->regs.halt = 0;     // re-enable TX transfer

    // configure data-parity-stop (low 4 bits of LCR)
    uint8_t data = 0b11;    // 8 data
    uint8_t parity = 0b0;   // no parity
    uint8_t stop = 0b0;     // 1 stop
    uint8_t settings = (parity << 3) | (stop << 2) | (data << 0);
    while (module.uart->regs.usr & USR_BUSY) ; // wait until uart not busy
    // clear low 4 bits, replace with settings 8-n-1
    module.uart->regs.lcr = (module.uart->regs.lcr & ~0b1111) | settings;

    module.uart->regs.mcr = 0;    // disable modem control
    module.uart->regs.ier = 0;    // disable interrupts by default
}

void uart_init(void) {
    static bool initialized = false;
    if (initialized) error("uart_init() should be called only once.");
    initialized = true;
    module.uart = NULL;
    // default to UART0 on pins PB8+9
    uart_reinit_custom(0, GPIO_PB8, GPIO_PB9, GPIO_FN_ALT6);
    uart_putstring("\n\n\n\n");
}

void uart_use_interrupts(handlerfn_t handler, void *client_data) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    int interrupt_source = INTERRUPT_SOURCE_UART0 + module.config.index;
    interrupts_register_handler(interrupt_source, handler, client_data);
    module.uart->regs.ier = 1;    // enable interrupts
}

unsigned char uart_recv(void) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    while (!uart_haschar()) ; // wait for char to arrive
    return module.uart->regs.rbr & 0xFF;
}

void uart_send(unsigned char byte) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    while ((module.uart->regs.usr & USR_TX_NOT_FULL) == 0) ;
    module.uart->regs.thr = byte & 0xFF;
}

void uart_flush(void) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    while ((module.uart->regs.usr & USR_BUSY) != 0) ;
}

bool uart_haschar(void) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    return (module.uart->regs.usr & USR_RX_NOT_EMPTY) != 0;
}

// RE: line endings
// canonial use is '\n' newline as sole line terminator (both read/write)
// but connected terminal may expect to receive a CR-LF sequence from Pi
// and may send a CR to Pi for return/enter key. uart_getchar and uart_putchar
// internally convert chars, client can simply send/receive newline
// Use uart_send/uart_recv to send/receive raw byte, no conversion

int uart_getchar(void) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    int ch = uart_recv();
    if (ch == '\r') {
        return '\n';    // convert CR to newline
    }
    return ch;
}

int uart_putchar(int ch) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    // convert newline to CR LF sequence by inserting CR
    if (ch == '\n') {
        uart_send('\r');
    }
    uart_send(ch);
    return ch;
}

int uart_putstring(const char *str) {
    if (module.uart == NULL) error("uart_init() has not been called!\n");
    int n = 0;
    while (str[n]) {
        uart_putchar(str[n++]);
    }
    return n;
}

void uart_start_error(void) {
    if (module.uart == NULL) {
        // if uart_init has not been called, there is no serial connection to read/write
        // All calls to uart operations are dead ends (that means no printf/assert!)
        // Force a call to uart_init here to enable reporting of problem
        // (otherwise lack of output is ultra mysterious)
        uart_reinit_custom(0, GPIO_PB8, GPIO_PB9, GPIO_FN_ALT6);
    }
    uart_putstring("\033[31;1m"); // red-bold
}

void uart_end_error(void) {
    uart_putstring("\033[0m\n"); // normal
}
