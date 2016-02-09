#include <stdio.h>

int main() { 
	unsigned a = 8;

#if 0
	if(a & 0xffff0000 != 0)
		printf("high bits are set! a=%d\n", a);
#endif

#if 1
	unsigned lo = 1;
	if((1U <<  (31U + lo)) != 0U)
		printf("hi!\n");
#endif
	return 0;
}
