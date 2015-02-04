/* periph.c */
unsigned int uart_lcr(void);
unsigned int uart_recv(void);
void uart_send(unsigned int c);
void hexstrings(unsigned int d);
void hexstring(unsigned int d);
void uart_init(void);
void timer_init(void);
unsigned int timer_tick(void);

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void BRANCHTO ( unsigned int );
void dummy ( unsigned int );
unsigned int GETPC ( void );
