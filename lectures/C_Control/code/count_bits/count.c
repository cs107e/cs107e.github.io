int main(void)
{
    unsigned int val = 0x3a;
    int count = 0;

    while (val != 0) {
        if (val & 1) count++;
        val = val >> 1;
    }
    return count;
}
