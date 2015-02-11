#include "fractal.h"

typedef void (*write_pixel_t)(uint_t x, uint_t y, uchar_t color);

void printdot_inc(unsigned x, unsigned y);
void printdot(unsigned x, unsigned y, unsigned color);

void enable_cache(void) {
        unsigned r;
        asm volatile ("MRC p15, 0, %0, c1, c0, 0" : "=r" (r));
        r |= 0x1800;
        asm volatile ("MCR p15, 0, %0, c1, c0, 0" :: "r" (r));
}


/***********************************************************************
 * Some fast-ish, not super accurate software versions of various math
 * functions.
 */

#define M_PI 3.1415926536897932384626

float floor(float f) {
	unsigned u = f;
	return u;
}

float fabs(float f) { return (f < 0.0) ? -f : f; }

//http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
float sin(float x) {
	const float B = 4/M_PI;
    	const float C = -4/(M_PI*M_PI);
	const float P = 0.225;

	float y = B * x + C * x * fabs(x);
	return P * (y * fabs(y) - y) + y;   // Q * y + P * y * abs(y)
}

float cos(float x) {
	x += M_PI/2.0;
	if(x > M_PI)
		x -= 2 * M_PI;  // wrap: cos(x) = cos(x-2pi);
	return sin(x);
}


/***********************************************************************
 * Baby versions of a few complex operator functions.
 */
struct complex {
	double r, i;
};

struct complex complex(double r, double i) {
	struct complex x;
	x.r = r; x.i = i;
	return x;
}

struct complex c_sq(struct complex x) {
	struct complex r;

	r.i = x.r*x.i * 2;
	r.r = x.r * x.r - x.i*x.i;

	return r;
}

struct complex c_add(struct complex a, struct complex b) {
	a.r += b.r;
	a.i += b.i;
	return a;
}
	

/*************************************************************************
 * p 119 of cliffor pickovers' book _computers, pattern, chaos and beauty 
 */
void chaotic_curls(uint_t x_pixels, uint_t y_pixels, uint_t test, uint_t N, 
					write_pixel_t write_pixel) {
	unsigned color, x_pix, y_pix;
	double x_delta = 2./ x_pixels ;
	double y_delta = 2./y_pixels;

	struct complex u = complex(-.74, .11);
	
	double rrz, iiz, sq;
	
	struct complex z;
	enable_cache() ;

	for(x_pix = 0, rrz = -1.; rrz < 1.; rrz += x_delta, x_pix++) {
		for(y_pix = 0, iiz = -1.; iiz < 1.; iiz += y_delta, y_pix++) {

			z = complex(rrz, iiz);

			int i;
			for(i = 0; i < N; i++) {
				z = c_add(c_sq(z), u);

				sq = z.i*z.i + z.r*z.r;
				if(sq > 4) {
					break;
				}
			}
			// this can be changed: 24 bit color.
			color = i + 100;
			
			if(test == 1) {
				if(i != N)
					write_pixel(x_pix, y_pix, color);
			} else if(test == 2) {
				if(fabs(z.r) < 2 && fabs(z.i) < 2)
					write_pixel(x_pix, y_pix, color);
			} else if(test == 3) {
				if(i != N && i % 2 == 0)
					write_pixel(x_pix, y_pix, color);
			}
		}
	}
}	


/*************************************************************************
 * Pretty, fairly fast.  p 165, ibid.
 */

// randomness=1 is a reasonable start.
void attractor(uint_t x_pixels, uint_t y_pixels, 
			double e, write_pixel_t write_pixel) {
	enable_cache() ;

	double xxmin = -2,
		xxmax=2,
		yymin = -2,
		yymax=2;
	int iter1 = 1000; // should add these as paramters.
	int iter2 = 5000;

	iter2 = 500;

	// this can be made faster by factoring.
	double xinc = x_pixels / (xxmax - xxmin);
	double yinc = y_pixels / (yymax - yymin);
	double a = 2.24, b = .43, c = -.65, d = -2.43;
	double x = 0.0, y = 0.0, z = 0.0;

	int i,j;
	for(j = 0; j < iter1; j++) {
		for(i = 0; i < iter2; i++) {
			double xx = sin(a*y) - z * cos(b*x);
			double yy = z * sin(c*x) - cos(d*y);
			double zz = e*sin(x);
			x = xx; y = yy; z = zz;

			if(xx < xxmax && xx > xxmin 
			&& yy < yymax && yy > yymin) {
				unsigned xxx = (xx - xxmin) * xinc;
				unsigned yyy = (yy - yymin) * yinc;
				write_pixel(xxx,yyy, 0);
			}
		}
	}
}

/*
 * some decent calls:
        chaotic_curls(w, h, 3, 100, write_pixel_julia);
   or
        chaotic_curls(w, h, 3, 100, write_pixel_julia_simple);
   or
        attractor(w, h, 1, write_pixel_attractor);
 */
#include "fb.h"
#include "graphics.h"

void frame_clear(fb_t fb, uint_t w, uint_t h) {
        int i;
        for(i = 0; i < h*w; i++) {
                fb_t p = &fb[i];
                p->r = p->g = p->b = 0xff;
                p->alpha = 0xff;
        }
}

void simple_attractor(double e) {
	fb_t fb = (void *)fb_writeable_buffer();
	uint_t w = fb_width();
	uint_t h = fb_height();

	void write_pixel(uint_t x, uint_t y, uchar_t _unused) {
        	if(x > w || y > h) // should not happen
			return;
        	volatile struct pixel *p = &fb[x + y*w];
        	p->r = 200;
        	p->b = p->g = 0;
	}


	frame_clear(fb, w, h);
	attractor(w, h, e, write_pixel);
}

void simple_julia(double e) {
	fb_t fb = (void *)fb_writeable_buffer();
	uint_t w = fb_width();
	uint_t h = fb_height();

	void write_pixel(uint_t x, uint_t y, uchar_t _unused) {
        	if(x > w || y > h) // should not happen
			return;
        	volatile struct pixel *p = &fb[x + y*w];
        	p->r = 200;
        	p->b = p->g = 0;
	}

	frame_clear(fb, w, h);
	attractor(w, h, e, write_pixel);
}

void simplest_chaos(int test, int n) {
	fb_t fb = (void *)fb_writeable_buffer();
	uint_t w = fb_width();
	uint_t h = fb_height();

	void write_pixel(uint_t x, uint_t y, uchar_t color) {
		volatile struct pixel *p = &fb[x + y*w];
        	p->r = color;
        	p->g = p->b = 0;
	}

	frame_clear(fb, w, h);
	chaotic_curls(w, h, test, n, write_pixel) ;
}

void simple_chaos(int test, int n) {
	fb_t fb = (void *)fb_writeable_buffer();
	uint_t w = fb_width();
	uint_t h = fb_height();

	void write_pixel(uint_t x, uint_t y, uchar_t color) {
        	volatile struct pixel *p = &fb[x + y*w];
        	// should do something more clever.
        	p->r = 0xff*color;
        	p->g = p->r * color;
        	p->b = p->g * color;

        	// don't let it be white.
        	if(p->r > p->g) {
                	if(p->r > p->b)
                        	p->r = 0;
                	else
                        	p->b = 0;
        	} else
                	p->g = 0;
	}

	frame_clear(fb, w, h);
	chaotic_curls(w, h, test, n, write_pixel) ;
}
