#ifndef UART_H
#define UART_H

/*
 * Hardware abstraction for a serial port (UART).
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include "interrupts.h"

// gpio pins for connecting uart TX/RX lines
#define UART_TX_GPIO GPIO_PB8
#define UART_RX_GPIO GPIO_PB9

/*
 * `uart_init`: Required initialization for module
 *
 * Initialize the UART module. A single call to `uart_init`
 * should made as part of the program initialization, before any
 * calls to other uart_ functions.  It is allowed, although unusual,
 * to call `uart_init` again to reset the UART state. A reset
 * will discard any pending data in the send/receive buffer.
 *
 * The uart requires exclusive use of GPIO pins PB8 (transmit)
 * and PB9 (receive).  Once the uart is initialized and in use,
 * your code should not further access these two pins.
 */
void uart_init(void);

/*
 * `uart_getchar`
 *
 * Obtains the next input character from the serial port and returns it.
 * If receive buffer is empty, will block until next character is received.
 *
 * @return    the character read or EOF if error or at end of input
 */
int uart_getchar(void);

/*
 * `uart_putchar`
 *
 * Outputs a character to the serial port.
 * If send buffer is full, will block until space available.
 *
 * @param ch   the character to write to the serial port
 * @return     the character written
 */
int uart_putchar(int ch);

/*
 * `uart_flush`
 *
 * Flushes any output characters pending in the send buffer.
 */
void uart_flush(void);

/*
 * `uart_haschar`
 *
 * Returns whether there is a character in the receive buffer.
 *
 * @return      true if character ready to be read, false otherwise
 */
bool uart_haschar(void);

/*
 * `uart_putstring`
 *
 * Outputs a string to the serial port by calling `uart_putchar`
 * on each character.
 *
 * @param str  the string to output
 * @return     the count of characters written or EOF if error
 */
int uart_putstring(const char *str);

/*
 * `uart_send`
 *
 * Outputs raw byte to the serial port. `uart_send` outputs the raw byte
 * with no translation (unlike `uart_putchar` which adds processing for
 * converting end-of-line markers). To send text character, use
 * `uart_putchar`; if raw binary data, use `uart_send`.
 *
 * @param byte   the byte to write to the serial port
 */
void uart_send(unsigned char byte);

/*
 * `uart_recv`
 *
 * Obtain raw byte from the serial port. `uart_recv` returns the raw
 * byte with no translation (unlike uart_getchar which adds processing
 * for converting end-of-line and end-of-file markers). To read text
 * character, use `uart_getchar`; if raw binary data, use `uart_recv`.
 *
 * @return   the byte read from the serial port
 */
unsigned char uart_recv(void);

/*
 * `uart_use_interrupts`
 *
 * @param handler   the handler function to call
 * @param client_data  to write to the serial port
 */
void uart_use_interrupts(handlerfn_t handler, void *client_data);

/*
 * `uart_start_error`,`uart_end_error`
 *
 * Output ANSI color code for red+bold on start error, restore to
 * normal on end error. Used by assert/error to highlight error
 * messages.
 */
void uart_start_error(void);
void uart_end_error(void);

#endif
