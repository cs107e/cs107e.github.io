/*
 * show effect of volatile
 */

int notmain(void)
{
    int val;
    int *ptr;

    ptr = &val;

    *ptr = 1;
    *ptr = 2;

    return val;
}
