#ifndef PRINTF_INTERNAL_H
#define PRINTF_INTERNAL_H

/*
 * Functions: signed_to_base, unsigned_to_base
 * -------------------------------------------
 * These helper functions convert a numeric value to a string 
 * representation in a particular base. For example, given the 
 * integer value `123` and base 10, its string representation is 
 * the sequence of four ASCII characters `1` `2` `3` `\0`, i.e. 
 * the string `"123"`. If the requested base were 16, the string 
 * representation is `"7b"`.
 *
 * The `val` argument is the integer value to convert.
 * If `signed_to_base` is called with a negative value, the output string 
 * will contain a leading minus sign.
 *
 * The `base` argument indicates whether the output string is to be represented 
 * in decimal (base 10) or hexadecimal (base 16).  No other bases are
 * supported.
 *
 * The `min_width` is the minimum number of characters in the output string.  
 * If the output string contains fewer characters than `min_width`, the output 
 * string is padded with leading zeros to bring it up to length `min_width`. 
 * If the  output string is already at least as long as `min_width`, the 
 * `min_width` argument is ignored (e.g. a min_width of 0 sets no minimum).
 * The minus sign should precede the zero padding and is included in the 
 * `min_width`. You may assume `min_width` is non-negative.
 *
 * The caller supplies the arguments `buf` and `bufsize`. `buf` is the address 
 * of the character array where the output string is to be written. `bufsize` 
 * is the size of the array. The functions must respect this size and never 
 * write more characters that fit in `buf`. If the output string would be 
 * too long, truncate to what fits, being sure to reserve one slot for the 
 * null terminator. The contents written to `buf` must be null-terminated. 
 *
 * The return value is the count of characters written to `buf` if there is 
 * space to fit the entire output string. If not, it returns
 * the count of characters that would have been written if there
 * were space. The null terminator is not included in the count. 
 */

int unsigned_to_base(char *buf, size_t bufsize, unsigned int val, int base, int min_width);

int signed_to_base(char *buf, size_t bufsize, int val, int base, int min_width);


#endif
