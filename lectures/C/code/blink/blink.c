#define GPIO  0x20200000
#define DELAY 0x3F0000

void main(void)
{
    volatile int i;
    int *r0;
    
    // initialize gpio20 function select for output
    r0 = (int*)GPIO;
    r0[0x08/4] = 1;

    while( 1 ) {

        // turn on led
        r0[0x1C/4] = 1 << 20;

        for( i = 0; i < DELAY; i++ )
            ;

        // turn off led
        r0[0x28/4] = 1 << 20;

        for( i = 0; i < DELAY; i++ )
            ;
    }
}
