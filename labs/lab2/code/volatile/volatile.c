int volatileint(void)
{
    volatile int *ptr = (int*)0x20200008;
    //int *ptr = (int*)0x20200008;
    int val = 0;
    int i;

    for( i = 0; i < 10; i++ )
       val = *ptr;
    return val;
}

