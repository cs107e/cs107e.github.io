/* 
 * copy.c
 */

char *
copy(const char *s)
{
    char scopy[128];
    char *s2 = scopy;

    while ((*s2++ = *s++) != 0)
         ;
    return scopy;
}

int notmain(void)
{
    char *s = copy("stanford");

    return s[0];
}
