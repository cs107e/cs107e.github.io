int x = 1;
const int const_x = 2;

int bss_x;
const int const_bss_x;

static int static_x = 3;
static const int static_const_x = 4;

static int static_bss_x;
static const int static_const_bss_x;

static int f(int a, int b, int c, int d)
{
    x = a;
    bss_x = b;
    static_x = c;
    static_bss_x = d;

    return static_x + static_const_x + static_bss_x + static_const_bss_x;
}

int main(void)
{
    return f(2, 3, 4, 5);
}
