main()
{
    int *r0;

    r0 = (int*)0x20200000; // GPIO BASE 
    r0[0x08/4] = 1;
    r0[0x1C/4] = 1 << 20;

    while(1) ;
}
