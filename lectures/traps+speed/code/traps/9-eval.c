#include <stdio.h>
 
int main( void ) {
	int a = 12;
  	int b = sizeof( ++a );
  	printf( "%d, %d\n", a , b);
  	return 0;
}
