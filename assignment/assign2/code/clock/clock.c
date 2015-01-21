#define SYSTIMER 0x20003004

/*
** the timer counter increments every 1 usec
*/
unsigned int timer ( void )
{
    volatile int *p = (int*)SYSTIMER;
    return *p;
}

/*
** delay by n usec
*/
void delay_us( unsigned int us )
{
    unsigned int start = timer();
    while( timer() - start < us )
        ;
}

/*
** initialiaze a GPIO pin
**
**  mode == 0 -> INPUT
**  mode == 1 -> OUTPUT
*/
void pinmode( int pin, int mode )
{
}

/*
** set a GPIO pin to 0 or 1
*/
void pinwrite( int pin, int value )
{
}

void main ( void )
{
}
