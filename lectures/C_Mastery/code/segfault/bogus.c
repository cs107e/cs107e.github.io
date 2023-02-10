// This buggy program tries to write to a bogus address
// what happens if you run this program on a hosted system?
// what happens if you run on bare metal Pi?
#include <stddef.h>
#include <stdio.h>

void peek_poke(int *ptr, int val)
{
    printf("\nProbing address %p\n", ptr);
    printf("\tRead value %x\n", *ptr);
    *ptr = val;
    printf("\tWrote value %x\n", *ptr);
}


int main(void)
{
    int n = 1;

    peek_poke(&n, 0x99994444);
    peek_poke(NULL, 5);

    void *unaligned = (char *)&n + 1;
    peek_poke(unaligned, 0);
    printf("value of n = %x\n", n);

    printf("Now I am become Death, the destroyer of worlds.\n\004");
    return 0;
}
