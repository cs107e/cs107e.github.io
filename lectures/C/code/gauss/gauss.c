#define MAX 100

void main()
{
    int sum = 0;
	for (int i = 1; i <= MAX; i++)
		sum += i;

    // configure GPIO 20 as output
    *(int *)0x20200008 = 1;

    for (int i = 0; i < sum; i++) {
    	// set GPIO 20 high
        *(int *)0x2020001C = 1 << 20;

    	int delay = 0x3ef000;
    	while (--delay != 0) ;

   		// clear GPIO 20
    	*(int *)0x20200028 = 1 << 20;

    	delay = 0x3ef000;
    	while (--delay != 0) ;
	}
}
