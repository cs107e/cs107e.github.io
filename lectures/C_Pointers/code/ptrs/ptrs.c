// This buggy program tries to write to a bogus address
// what happens if you run this program on a honsted system?
// what happens on baremetal Pi?

int main(void)
{
    int *my_ptr = (int *)0x20;

    *my_ptr = 1;
    return 0;
}
