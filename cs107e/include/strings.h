#ifndef _STRINGS_H
#define _STRINGS_H

#include <stddef.h>

/*
 * Write `n` bytes of value `c` (converted to an unsigned char) to the memory
 * area pointed to by `s`.
 *
 * Return its first argument: `s`.
 */
void *memset(void *s, int c, size_t n);

/*
 * Copy `n` bytes from the memory area `src` to the memory area `dst`. If `dst`
 * and `src` overlap, the behavior is undefined.
 *
 * Returns its first argument: `dst`.
 */
void *memcpy(void *dst, const void *src, size_t n);

/*
 * Report the length of string `s`.
 *
 * Returns the number of characters that precede the null-terminator.
 */
size_t strlen(const char *s);

/*
 * Lexicographically compare the null-terminated strings `s1` and `s2`.
 *
 * Returns an integer value greater than, equal to, or less than 0, according
 * to whether the string `s1` is greater than, equal to, or less than 
 * the string `s2`. Comparison is done as unsigned characters.
 */
int strcmp(const char *s1, const char *s2);

/*
 * Convert the digit characters in `str` to the corresponding unsigned integer
 * value. If `str` begins with the prefix "0x", the characters of `str` will
 * be interpreted as hexadecimal digits (base 16); otherwise the characters
 * are interpreted as decimal digits (base 10). No other bases are supported.
 * The hex letter digits are to be specified in lowercase.
 *
 * `strtonum` processes the characters of `str`, stopping at the first
 * character that is not a valid digit in the base or at the terminating
 * null (whichever comes first).  The function is not required to support 
 * leading spaces or a plus/minus sign. Such characters can be
 * treated as invalid and stop the conversion.
 *
 * The argument `endptr` is an output parameter optionally used to  
 * communicate back to the caller what characters remain in `str` after having 
 * "consumed" the digit characters. A caller can pass NULL for `endptr`
 * if they do not need this information.
 *
 * If `endptr` is not NULL, *endptr is updated to point to the character
 * in `str` where conversion stopped. This is either the address of the
 * first invalid character in `str` or the address of the terminating null
 * if all characters in `str` are valid digits.
 *
 * Returns the result of the conversion as an unsigned integer. Returns 0
 * if the first character of `str` is not a valid digit.
 */
unsigned int strtonum(const char *str, const char **endptr);

/*
 * Size-bounded string concatenation. Append the null-terminated string `src`
 * to the end of `dst`. Appends at most `maxsize - strlen(dst) - 1` bytes, and
 * null-terminates `dst`. If `dst` and `src` overlap, the behavior is undefined.
 *
 * Returns the initial length of `dst` plus the length of `src`. This is 
 * equal to the number of characters that would have been written to `dst` if 
 * there were space.
 */
size_t strlcat(char *dst, const char *src, size_t maxsize);

#endif
