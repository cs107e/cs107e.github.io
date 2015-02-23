#include "fb.h"

#define MAILBOX_FULL      0x80000000
#define MAILBOX_EMPTY     0x40000000
#define MAILBOX_START     0x2000B880
#define GPU_MEM_OFFSET    0x40000000

enum {
  FB_WIDTH      = 1280,
  FB_HEIGHT     = 960,
  FB_BYTE_DEPTH =   4
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
} fb_config_t;


static volatile fb_config_t config __attribute__ ((aligned(16)));
char* buffer;

int fb_init() {
  config.width = config.virtual_width = FB_WIDTH;
  config.height = config.virtual_height = FB_HEIGHT;
  config.depth = FB_BYTE_DEPTH * 8;
  config.x_offset = 0;
  config.y_offset = 0;

  mailbox_t* mbox = (mailbox_t*)MAILBOX_START;

  while (mbox->status & MAILBOX_FULL) {}

  mbox->write = ((unsigned int)&config) + GPU_MEM_OFFSET + 1;

  while (1) {
    while (mbox->status & MAILBOX_EMPTY) {}
    if ((mbox->read & 0xF) == 1) {
      buffer = (char*)config.pointer;
      return 1;
    }
  }
  return 0;
}

char* fb_writeable_buffer() {
  return buffer;
}

unsigned int fb_byte_depth() {
  return config.depth / 8;
}

unsigned int fb_height() {
  return config.height;
}

unsigned int fb_width() {
  return config.width;
}

