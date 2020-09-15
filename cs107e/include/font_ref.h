#ifndef FONT_REF
#define FONT_REF
#include "font.h"
const font_t *ref_font_get_font(void);
void ref_font_set_font(font_t *f);
size_t ref_font_get_height(void);
size_t ref_font_get_width(void);
size_t ref_font_get_size(void);
bool ref_font_get_char(char ch, unsigned char buf[], size_t buflen);
#endif
