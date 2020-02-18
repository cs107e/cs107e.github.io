#ifndef FONT_H
#define FONT_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Functions that provide bitmaps of ASCII characters.
 *
 * Each character is represented as an image that has
 * font_get_height() rows and font_get_width() columns.
 * Each pixel of the image is represented as one byte.
 * The byte for an "on" pixel is 0xFF, and the byte for 
 * "off" pixel is 0x00.
 *
 * You do not have to implement this interface: the source code for
 * its implementation is provided as part of assignment 6.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: Mar 25 2016
 */

typedef struct  {
    unsigned char first_char, last_char;
    size_t char_width, char_height;
    unsigned char pixel_data[];
} font_t;


/*          
 * Get the current font.
 */  
const font_t *font_get_font(void);

/*          
 * Set the current font.
 */  
void font_set_font(font_t *f);

/*          
 * Get the height in pixels of a character.
 *              
 * @return    the character height in pixels
 */  
size_t font_get_height(void);

/*          
 * Get the width in pixels of a character.
 *              
 * @return    the character width in pixels
 */  
size_t font_get_width(void);

/*          
 * Get the total number of bytes needed to store a character
 * image. This is equal to character height * character width.
 *              
 * @return    the size in bytes
 */  
size_t font_get_size(void);

/*          
 * Fill in the image of character `ch` into the buffer `buf`.
 * `buf` is an array of bytes of length width * height (see
 * font_get_size(). Each 'on' pixel has value 0xff, 'off' pixel has 
 * value 0x0.
 *              
 * @param ch    the requested character
 * @param buf   the buffer in which to place the image of the character.
 * @param buflen the length of `buf`.
 *              `buflen` should be equal to value returned by font_get_size().
 *
 * @return      returns true when successfully filled `buf`, false otherwise.
 *              Failure is when `ch` is not available in this font or `buflen` 
 *              does not equal the value returned by font_get_size()
 */  
bool font_get_char(char ch, unsigned char buf[], size_t buflen);

#endif
