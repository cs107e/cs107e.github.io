/* string.c
 * --------
 * A nonsense program that only exists to give you a chance
 * to explore relationships between arrays/pointers and
 * experiment with different ways to access memory.
 */

int
strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s)
        ;
    return(s - str);
}


char *
strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while ((*s++ = *s2++) != 0)
         ;
    return s1;
}

// chop_to_front and chop_to_back are intended to truncate 
// half of the passed string
// chop_to_front keeps the front half and truncates the rest.
// chop_to_back keeps the back half and discards the front. 
// As written below, the front version does its job, yet 
// back has no effect. Both are attempting to make a persistent 
// change, yet only one is successful in its attempt. What's the 
// difference between the two?  How can chop_to_back be modified
// so that it works as intended?

void chop_to_front(char *str)
{
    str[strlen(str)/2] = '\0'; // write null char at midpoint to truncate suffix
}

void chop_to_back(char *str)
{
    str = str + strlen(str)/2; // assign ptr to midpoint to remove prefix
}

int notmain(void)
{
    char buffer[100];

    strcpy(buffer, "stanford");
    chop_to_front(buffer);
    
    strcpy(buffer, "stanford");
    chop_to_back(buffer);

    return buffer[0];
}
