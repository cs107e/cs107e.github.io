struct pixel {
        unsigned char r,
                      g,
                      b,
                      alpha;
};

typedef volatile struct pixel *fb_t;
typedef unsigned uint_t;
typedef unsigned char uchar_t;
void fb_clear(fb_t fb, uint_t h, uint_t w, uint_t v);
