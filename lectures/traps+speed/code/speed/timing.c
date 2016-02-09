#include "video.h"
#include "printf.h"
#include "rpi.h"

struct pixel {
        unsigned char r,
                      g,
                      b,
                      alpha;
};
typedef unsigned uint_t;
typedef unsigned char uchar_t;
typedef volatile struct pixel *fb_t;

static fb_t fb;
static uint_t h, w;


void write_screen(unsigned char v) {
	volatile int i;

	for(i = 0; i < fb_width() * fb_height(); i++) {
		fb_t p = &fb[i];
		p->r = p->g = p->b = v;
		p->alpha = 0xff;
	}
}


// took 604454 ticks
void write_hack_probably_broken(unsigned char v) {
	int i;

	unsigned n = fb_width() * fb_height();

	fb_t p;
	for(i = 0; i < n; i++) {
		p = &fb[i];
		p->r = p->g = p->b = v;
		p->alpha = 0xff;
	}
}



























void write_screen_precomp(unsigned char v) {
	volatile int i;
	unsigned n = fb_width() * fb_height();

	for(i = 0; i < n; i++) {
		fb_t p = &fb[i];
		p->r = p->g = p->b = v;
		p->alpha = 0xff;
	}
}






























void write_screen_no_v(unsigned char v) {
	int i;
	unsigned n = fb_width() * fb_height();

	for(i = 0; i < n; i++) {
		fb_t p = &fb[i];
		p->r = p->g = p->b = v;
		p->alpha = 0xff;
	}
}






























void write_screen_word(unsigned char v) {
	int i;
	unsigned n = fb_width() * fb_height();

	struct pixel px;
	px.r = px.g = px.b = v;
	px.alpha = 0xff;

	assert(sizeof px == sizeof(unsigned));

	unsigned u = *(unsigned *)&px;
	for(i = 0; i < n; i++) {
		fb_t p = &fb[i];
		*(unsigned *)p = u;
	}
}







































void write_screen_ll(unsigned char v) {
	int i;
	unsigned n = fb_width() * fb_height();

	struct pixel px;
	px.r = px.g = px.b = v;
	px.alpha = 0xff;

	assert(sizeof px == sizeof(unsigned));

	unsigned long long u = *(unsigned *)&px;
	u = (u << 32) | u;
	for(i = 0; i < n; i+=2) {
		unsigned long long *p = (void*)&fb[i];
		*p = u;
	}
}
































void system_memory_write_barrier() {
  // DSB: data synchronization barrier
  asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory");
}








void write_screen_induction(unsigned char v) {
        int i;
        unsigned n = fb_width() * fb_height();

        struct pixel px;
        px.r = px.g = px.b = v;
        px.alpha = 0xff;
        unsigned long long u = *(unsigned *)&px;
        unsigned long long uu = u << 32 | u;

        volatile unsigned long long *p = (void*) fb;
	n = n/2;
        for(i = 0; i < n; i++, p++)
                *p = uu;

	//system_memory_write_barrier();
}











void write_screen_unroll(unsigned char v) {
        int i;
        unsigned n = fb_width() * fb_height();

        struct pixel px;
        px.r = px.g = px.b = v;
        px.alpha = 0xff;
        unsigned long long u = *(unsigned *)&px;
        unsigned long long uu = u << 32 | u;

        unsigned long long *p = (void*) fb;

        n = n/2;
        assert(n%8 == 0);
        for(i = 0; i < n; i+=8, p+=8)  {
                p[0] = uu;
                p[1] = uu;
                p[2] = uu;
                p[3] = uu;
                p[4] = uu;
                p[5] = uu;
                p[6] = uu;
                p[7] = uu;
        }
}





















void assign8(void *, unsigned u, unsigned n);

void write_screen_asm(unsigned char v) {
        struct pixel px;
        px.r = px.g = px.b = v;
        px.alpha = 0xff;
        unsigned u = *(unsigned *)&px;

        unsigned n = fb_width() * fb_height();
        assert(n%8 == 0);
        assign8((void*)fb, u, n/8);
}


































void assign8_induct(void *, unsigned u, unsigned *end);

void write_screen_asm_induct(unsigned char v) {
        struct pixel px;
        px.r = px.g = px.b = v;
        px.alpha = 0xff;
        unsigned u = *(unsigned *)&px;

        unsigned n = fb_width() * fb_height();
        assert(n%8 == 0);
        assign8_induct((void*)fb, u, (unsigned*)fb+n);
}


unsigned gettime(void) { return GET32(0x20003004); }
#define TIME(cmd, val, iter) do {                            \
        printf("running: " #cmd "\n");                  \
        int i;                                          \
        for(i = 0; i < iter; i++) {                     \
                unsigned start = gettime();             \
                        cmd(val);  cmd(0x00); 		\
                unsigned total = gettime() - start;     \
                printf("took %d ticks\n", total);       \
        }                                               \
} while(0)
	
void notmain() {
	fb_init();
	
	fb = (void*)fb_writeable_buffer();
	h = fb_height();
	w = fb_width();

	TIME(write_screen, 0xf0, 2);
#if 0
	TIME(write_screen_precomp, 0xff, 4);
	TIME(write_screen_no_v, 0xff, 4);
	TIME(write_screen_word, 0xff, 4);
	TIME(write_screen_ll, 0xff, 10);
#endif
	TIME(write_screen_induction, 0xff, 4);
	TIME(write_screen_unroll, 0xff, 4);
#if 0
	TIME(write_hack_probably_broken, 0xf0, 2);
	TIME(write_screen_unroll, 0xff, 4);
	TIME(write_screen_asm, 0xff, 4);
	TIME(write_screen_asm_induct, 0xff, 4);
#endif

	TIME(write_screen_asm, 0xff, 4);
	enable_cache();
	printf("caching!  wow!  wow!\n");
	TIME(write_screen, 0xf0, 2);
	TIME(write_screen_asm, 0xff, 4);
#if 0
	TIME(write_screen_precomp, 0xff, 4);
	TIME(write_screen_no_v, 0xff, 4);
	TIME(write_screen_word, 0xff, 4);
	TIME(write_screen_ll, 0xff, 10);
	TIME(write_screen_induction, 0xff, 4);
	TIME(write_screen_unroll, 0xff, 4);
#endif
#if 0
#endif
#if 0
	TIME(write_screen_asm, 0xff, 4);
	enable_cache();
	printf("caching!  wow!  wow!\n");
	TIME(write_screen_asm, 0xff, 4);
	TIME(write_screen_asm_induct, 0xff, 4);
#endif
	delay(10000000);
	reboot();
}
