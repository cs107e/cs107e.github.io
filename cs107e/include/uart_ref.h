#ifndef UART_REF
#define UART_REF
#include "uart.h"
void ref_uart_init(void);
int ref_uart_getchar(void);
int ref_uart_putchar(int ch);
void ref_uart_flush(void);
bool ref_uart_haschar(void);
int ref_uart_putstring(const char *str);
void ref_uart_send(unsigned char byte);
unsigned char ref_uart_recv(void);
#endif
