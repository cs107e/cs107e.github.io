#include <stdio.h>

int main() { 
	int a = 1, b = 2;

#if 0
	a---b;
	printf("a=%d, b=%d\n", a, b);
#endif

	a = 1; b = 1;
	a-- - --b;

	b-1;
	b-1;
	b-1;
	b-1;
	b-1;
	b-1;
	b-1;
	printf("a=%d, b=%d\n", a, b);


#if 0
	a = 1;
	a =- 2;
	printf("a = %d\n", a);

	a = 2;  b = 20;
	int *p = &a;
	b = a /*p          /* p points at the divisor */;
	printf("b = %d\n", b);
#endif

	return 0;
}
