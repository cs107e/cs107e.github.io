// Program to count the "on" bits in argument

int main(unsigned arg)
{
    int count = 0;
    do {
        count += arg & 1;
        arg >>= 1;
    } while (arg);
    return count;
}
