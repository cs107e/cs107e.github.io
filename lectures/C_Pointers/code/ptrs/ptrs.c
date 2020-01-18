

int main(int argc, char *argv[])
{
    int *my_ptr = (int *)0x20;

    // what happens if access invalid address on baremetal? on hosted system?
    *my_ptr = 1;
    return 0;
}
