#define GPIO  0x20200000
#define SECOND 0x3F0000

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

void dot(void)
{
    pinwrite(1);
    delay(SECOND/5);
    pinwrite(0);
    delay(SECOND/5);
}

void dash(void)
{
    pinwrite(1);
    delay(SECOND/2);
    pinwrite(0);
    delay(SECOND/5);
}

void sos(void)
{
    dot();
    dot();
    dot();
    delay(SECOND/2);
    dash();
    dash();
    dash();
    delay(SECOND/2);
    dot();
    dot();
    dot();
    delay(SECOND);
}


void newmain(void)
{
    pinmode(1);
    while(1)
        sos();
}
