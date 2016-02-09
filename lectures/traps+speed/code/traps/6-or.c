
#include <stdio.h>

int main() { 
	int p[10];
	int i = 1;

	if(i > 10 | p[i] != 10)
		printf("not found\n");
	else
		printf("found!  %d\n", i);

	return 0;
}
