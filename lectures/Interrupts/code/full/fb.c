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

void commit_config();

static volatile fb_config_t config __attribute__ ((aligned(16)));
char* buffer;
unsigned int active_buffer;

int fb_init() {
  active_buffer = 0;

  config.width = config.virtual_width = FB_WIDTH;
  config.height = FB_HEIGHT;
  config.virtual_height = 2 * FB_HEIGHT;
  config.depth = FB_BYTE_DEPTH * 8;
  config.x_offset = 0;
  config.y_offset = 0;

  commit_config();
  return 1;
}

char* fb_writeable_buffer() {
  return buffer + ((active_buffer ^ 1) * FB_WIDTH *
                   FB_HEIGHT * FB_BYTE_DEPTH);
}
char* fb_readonly_buffer() {
  return buffer + (active_buffer * FB_WIDTH *
                   FB_HEIGHT * FB_BYTE_DEPTH);
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

void fb_clear() {
  int i;
  unsigned int *b = (unsigned int*)fb_writeable_buffer();
  for (i = 0; i < (FB_WIDTH * FB_HEIGHT); i++) {
    b[i] = 0x00000000;
  }
}

void fb_draw() {
  active_buffer ^= 1;
  config.y_offset = FB_HEIGHT * active_buffer;
  commit_config();
}

void commit_config() {
  mailbox_t* mbox = (mailbox_t*)MAILBOX_START;
  memory_read_barrier();
  while (mbox->status & MAILBOX_FULL) {}

  mbox->write = ((unsigned int)&config) + GPU_MEM_OFFSET + 1;
  memory_write_barrier();
  while (1) {
    while (mbox->status & MAILBOX_EMPTY) {}
    if ((mbox->read & 0xF) == 1) {
      buffer = (char*)config.pointer;
      return;
    }
  }
}
