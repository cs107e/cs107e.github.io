#include <stdint.h>
#include <stdio.h>

int main(void)
{
    int32_t x = -1;
    uint32_t y = x;

    printf("x = 0x%08x (%d) -> y = 0x%08x (%d)\n", x, x, y, y);
}
