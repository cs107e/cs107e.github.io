#ifndef UART_H
#define UART_H

void uart_init ( void );
int uart_getc ( void );
void uart_putc ( unsigned c );
void uart_flush ( void );

#endif
