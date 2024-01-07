#ifndef STRINGS_H
#define STRINGS_H

/*
 * Functions for handling strings.
 *
 * Students implement this module in assignment 3
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stddef.h>

/*
 * `memset`
 *
 * Write `n` bytes of value `val` to the memory area `dst`.
 * Due to historical artifact, `val` parameter has type `int` even
 * though the argument is treated as `unsigned char`. The
 * least significant byte of `val` is copied `n` times.
 *
 * @param dst   address of memory location
 * @param val   the byte value to replicate
 * @param n     the number of bytes to write
 * @return      argument `dst`
 */
void *memset(void *dst, int val, size_t n);

/*
 * `memcpy`
 *
 * Copy `n` bytes of data from the memory area `src` to the
 * memory area `dst`. It is a client error to call memcpy on
 * `dst` and `src` areas that overlap. In such a case,
 * the behavior is undefined.
 *
 * @param dst   address of memory location area to write
 * @param src   address of memory location area to read
 * @param n     the number of bytes to copy
 * @return      argument `dst`
 */
void *memcpy(void *dst, const void *src, size_t n);

/*
 * `strlen`
 *
 * Compute the length of string `str`.
 *
 * @param str   null-terminated string
 * @return      the count of characters preceding terminating '\0' character
 */
size_t strlen(const char *str);

/*
 * `strcmp`
 *
 * Lexicographically compare the null-terminated strings `s1` and `s2`.
 *
 * The function result indicates whether string `s1` is less than (negative),
 * equal to (zero), or greater than (positive) string `s2`.
 * Comparison is done as unsigned characters.
 *
 * @param s1, s2    null-terminated strings
 * @return          negative, zero, or positive result of comparing `s1` to `s2`
 */
int strcmp(const char *s1, const char *s2);

/*
 * `strtonum`
 *
 * Convert the digit characters in `str` to the corresponding unsigned long
 * value. If `str` begins with the prefix "0x", the characters of `str` will
 * be interpreted as hexadecimal digits (base 16); otherwise the characters
 * are interpreted as decimal digits (base 10). No other bases are supported.
 * The hex letter digits are accepted in both upper and lowercase.
 *
 * `strtonum` processes the characters of `str`, stopping at the first
 * character that is not a valid digit in the base or at the terminating
 * '\0' (whichever comes first).  The function is not required to support
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
 * first invalid character in `str` or the address of the terminating '\0'
 * if all characters in `str` are valid digits.
 *
 * The function result is the converted value or 0 if the first character of
 * `str` is not a valid digit and no conversion was possible.
 *
 * The function only needs to handle converting numbers that are within
 * the range representable as `unsigned long`.
 *
 * @param str       null-terminated string to convert
 * @param endptr    output parameter, will point to character after conversion end
 * @return          result of numeric conversion (or 0 if first char is invalid)
 */
unsigned long strtonum(const char *str, const char **endptr);

/*
 * `strlcat`
 *
 * Size-bounded string concatenation. Append the null-terminated string `src`
 * to the end of `dst`. `strlcat` appends at most `dstsize - strlen(dst) - 1`
 * bytes, and null-terminates `dst`. If `dst` and `src` overlap,
 * the behavior is undefined.
 *
 * The function result is the initial length of `dst` plus the length of
 * `src`, i.e. the final length of `dst` if there were space to append
 * all of `src`.
 *
 * If there is no null terminator within the first `dstsize` characters of
 * `dst`, either `dstsize` is incorrect or `dst` is not a proper string.
 * In such cases, nothing is written to `dst` and the return result is
 * `dstsize + strlen(src)`
 *
 * @param dst       destination buffer containing null-terminated string
 * @param str       null-terminated string to append to destination
 * @param dstsize   size of the dst buffer
 * @return          final length of dst if there were space to append all of src
 */
size_t strlcat(char *dst, const char *src, size_t dstsize);

#endif
