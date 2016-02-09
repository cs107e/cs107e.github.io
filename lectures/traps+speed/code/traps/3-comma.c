#include <stdio.h>

int main() { 
//	printf("%d\n", (1,2,3,4,5,6));

	printf("%d\n", ({ printf("hello world!\n");  6; }));
	return 0;
}
