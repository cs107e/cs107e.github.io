int x = 1;
const int const_x = 2;

int a[10];
static int y = 3;
static const int const_y = 4;

int z;
static int w;

static int f(int a, int b)
{
   x = a;
   y = b;
   z = a+b;
   w = a-b;
   return const_x + x + const_y + y + z + w;
}

int main(void)
{
    return f(2,3);
}
