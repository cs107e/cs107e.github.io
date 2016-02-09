/* Implement frame buffer from video.h here. */

#include "video.h"
#include "printf.h"

#define MAILBOX_FULL   (1<<31)
#define MAILBOX_EMPTY  (1<<30)
#define MAILBOX_START  0x2000B880
#define GPU_MEM_OFFSET    0x40000000

enum {
#if 0
  VIDEO_WIDTH      = 1024,
  VIDEO_HEIGHT     = 768,
#endif

#if 1
  VIDEO_WIDTH      = 1184,
  VIDEO_HEIGHT = 624,
#else

  // phils
  VIDEO_WIDTH      = 1280,
  VIDEO_HEIGHT     = 960,
#endif
/*
  
  VIDEO_WIDTH      = 1280,
  VIDEO_HEIGHT     = 1024,

  VIDEO_WIDTH      = 1920,
  VIDEO_HEIGHT     = 1020,
*/

  VIDEO_BYTE_DEPTH =   32
};

typedef volatile struct {
  unsigned read;
  unsigned padding[3];
  unsigned peek;
  unsigned sender;
  unsigned status;
  unsigned configuration;
  unsigned write;
} mailbox_t;

typedef volatile struct {
	unsigned width;
  	unsigned height;
  	unsigned virtual_width;
  	unsigned virtual_height;
  	unsigned pitch;
  	unsigned depth;
  	unsigned x_offset;
  	unsigned y_offset;
  	unsigned pointer;
  	unsigned size;
	// I put this in b/c it *seems* that PI firmware adds new fields by
	// just appending them on existing structures.
	unsigned padding[64];   
 
} __attribute__ ((aligned(16))) fb_config_t;

static volatile fb_config_t config __attribute__ ((aligned(16)));
static volatile fb_config_t* cp = &config;

static volatile mailbox_t *mbox =  (void*)0x2000B880;
 
#if 0
void memset(void *_p, unsigned char v, unsigned n) {
	unsigned char *p = _p;
	int i;

	while(n-- > 0)
		*p++ = v;
}
	memset(cp, 0, sizeof *cp);
#endif


unsigned int fb_byte_depth() { return cp->depth; }
unsigned int fb_height() { return cp->height; }
unsigned int fb_width() { return cp->width; }

static unsigned uncached(volatile void *cp) { 
	return (unsigned)cp | GPU_MEM_OFFSET; 	
}
volatile char* fb_writeable_buffer() {
	return (void*)(cp->pointer); }
 

#include "mem-barrier.h"

enum { FB = 1,   VC = 8 };

void write_mailbox(volatile void *data, unsigned channel) {
	dmb();
	while(mbox->status & MAILBOX_FULL)
		mb();

	assert((unsigned)data%16 == 0);
	mbox->write = uncached(data) | channel;
	dmb();
}

unsigned read_mailbox(unsigned channel) {
	while(1) { 
		dmb();
		while(mbox->status & MAILBOX_EMPTY)
			mb();

		unsigned v = mbox->read;
		// should check the status.
		if((v & 0xf) == channel)
			return v;
	}
	return 0;
}


void get_dim(unsigned *w, unsigned *h) {
	volatile unsigned int mailbuffer[256] __attribute__((aligned (16)));

	/* Get the display size */
	mailbuffer[0] = 8 * 4;		// Total size
	mailbuffer[1] = 0;		// Process Request
	mailbuffer[2] = 0x40003;	// Display size
	mailbuffer[3] = 8;		// Buffer size
	mailbuffer[4] = 0;		// Request size
	mailbuffer[5] = 0;		// Space for horizontal resolution
	mailbuffer[6] = 0;		// Space for vertical resolution
	mailbuffer[7] = 0;		// End tag

	// get width and height
	write_mailbox(mailbuffer, VC);
	unsigned var = read_mailbox(VC);
	var++; // ugh: unused

	/* Valid response in data structure */
	if(mailbuffer[1] != 0x80000000)
		panic("invalid response: got %x", mailbuffer[1]);

	*w = mailbuffer[5];
	*h= mailbuffer[6];

	if(!*w || !*h) {
		debug("didn't get dimensions of screen");
		*w = VIDEO_WIDTH;
		*h = VIDEO_HEIGHT;
	}

	debug("H = %d, W size = %d\n", *h, *w);
}

int fb_init() { 
	unsigned v;
	uart_init();

	unsigned w,h;
	get_dim(&w, &h);

	memset((void*)cp, 0, sizeof *cp);
	
	cp->width = cp->virtual_width = w;
	cp->height = cp->virtual_height = h;

//	AssertNow(offsetof(mailbox_t, read)== 0);
	// pitch is set by the GPU
	cp->depth = VIDEO_BYTE_DEPTH;
	cp->x_offset = cp->y_offset = 0;

#if 1
	write_mailbox(cp, FB);
	v = read_mailbox(FB);
#else
	debug("waiting for mailbox\n");
	while(mb->status & MAILBOX_FULL)
		debug("was full: waiting\n");

	while(1) { 

		debug("got mailbox\n");
		mb->write = uncached(cp) | 1;
	
		debug("waiting to recv mailbox\n");
		while(mb->status & MAILBOX_EMPTY)
			debug("was empty: waiting\n");
	
		v = mb->read;
		debug("got read val from GPU = %x, status=%x\n", v,v&~1);

		if((v & 0xff) == 1) {
			if((v & ~1))
				panic("error from GPU: status=%x\n", v&~1);
			break;
		}
		debug("not what we wanted\n");
	}
#endif
	printf("framebuffer is at %x, size = %d pitch =%d\n", 
		cp->pointer, cp->size, cp->pitch);
	
//	reboot();
	return v;
}
