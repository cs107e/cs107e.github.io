#ifndef STRINGS_REF
#define STRINGS_REF
#include "strings.h"
void *ref_memset(void *s, int c, size_t n);
void *ref_memcpy(void *dst, const void *src, size_t n);
size_t ref_strlen(const char *s);
int ref_strcmp(const char *s1, const char *s2);
unsigned int ref_strtonum(const char *str, const char **endptr);
size_t ref_strlcat(char *dst, const char *src, size_t maxsize);
#endif
