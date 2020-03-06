#include <stdint.h>
#include <stdio.h>

int main(void)
{
    int32_t x = -1;
    uint32_t y = x;

    printf("%d -> %u \n", x, y);
}
