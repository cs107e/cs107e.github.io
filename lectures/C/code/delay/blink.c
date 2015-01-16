#define GPIO  0x20200000
#define DELAY 0x3F0000

// only works for GPIO 20
void pinmode(int mode)
{
    int *r0 = (int*)GPIO;
    if( mode )
        r0[0x08/4] = 1;
}

// only works for GPIO 20
void pinwrite(int val)
{
    int *r0 = (int*)GPIO;
    if( val ) 
        r0[0x1C/4] = 1 << 20; // set to 3.3V
    else
        r0[0x28/4] = 1 << 20; // set to 0.0V
}

void delay(int n)
{
    volatile int i;
    for( i = 0; i < n; i++ )
        ;
}

void pulse(void)
{
    pinwrite(1);
    delay(DELAY);
    pinwrite(0);
    delay(DELAY);
}

void newmain(void)
{
    pinmode(1);
    while( 1 )
        pulse();
}
