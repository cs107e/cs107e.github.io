extern int width;
extern void *p;

void f()
{
    unsigned int (*im)[width] = p;
    im[0][0] = 0;
}

void main(void) 
{
    f();
}

