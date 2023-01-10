#pragma once
#include "font.h"
size_t ref_font_get_glyph_height(void);
size_t ref_font_get_glyph_width(void);
size_t ref_font_get_glyph_size(void);
bool ref_font_get_glyph(char ch, unsigned char buf[], size_t buflen);
