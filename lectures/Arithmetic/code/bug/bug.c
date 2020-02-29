#include <stdint.h>
#include <stdio.h>

int main(void)
{
    int a = -20;
    unsigned int b = 6;

    if( a < b )
        printf("-20 < 6 - all is well\n");
    else
        printf("-20 >= 6 - omg \n");
}
