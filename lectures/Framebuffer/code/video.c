#include "video.h"

#define MAILBOX_FULL      0x80000000
#define MAILBOX_EMPTY     0x40000000
#define MAILBOX_START     0x2000B880
#define GPU_MEM_OFFSET    0x40000000

enum {
  VIDEO_WIDTH      = 640,
  VIDEO_HEIGHT     = 480,
  VIDEO_BYTE_DEPTH =   4
};

typedef struct {
  unsigned int read;
  unsigned int padding[3];
  unsigned int peek;
  unsigned int sender;
  unsigned int status;
  unsigned int configuration;
  unsigned int write;
} mailbox_t;

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int virtual_width;
  unsigned int virtual_height;
  unsigned int pitch;
  unsigned int depth;
  unsigned int x_offset;
  unsigned int y_offset;
  unsigned int pointer;
  unsigned int size;
} video_config_t;


static volatile video_config_t config __attribute__ ((aligned(16)));
 
int video_init() { 
}

char* video_writeable_buffer() { 
}

unsigned int video_byte_depth() {
}

unsigned int video_height() {
}

unsigned int video_width() {
}

