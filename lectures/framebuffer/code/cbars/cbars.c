#include <stdio.h>
#include "timer.h"
#include "uart.h"
#include "mailbox.h"
#include "fb.h"

// The top 7 bars: the last bar is a little wide, so it
// has two entries in the 8 entry array.
static unsigned int bars[] = {0xFFC0C0C0,
                              0xFF00C0C0,
                              0xFFC0C000,
                              0xFF00C000,
                              0xFFC000C0,
                              0xFF0000C0,
                              0XFFC00000,
                              0XFFC00000
                             };

// The 7 castellations in the middle. The last bar is a little
// wide, so it has two entries in the 8 entry array.
static unsigned int castellations[] = {0xFFC00000,
                                       0xFF131313,
                                       0xFFC000C0,
                                       0xFF131313,
                                       0xFFC0C000,
                                       0xFF131313,
                                       0xFFC0C0C0,
                                       0xFFC0C0C0
                                      };

void cbars(unsigned *fbptr, unsigned width, unsigned height)
{
  unsigned int (*fb)[1280] = (unsigned int(*)[1280])fbptr;

  unsigned int rx, ry;
  for (ry = 0; ry < height; ry++) {
    if (ry < 720) {
      for (rx = 0; rx < width; rx++) {
        unsigned int index = rx / 180;
        fb[ry][rx] = bars[index];
      }
    } else if (ry < 768 ) {
      for (rx = 0; rx < width; rx++) {
        unsigned int index = rx / 180;
        fb[ry][rx] = castellations[index];
      }
    } else {
      // The bottom pattern has a different geometry so just
      // brute force it with the X offsets.
      for (rx = 0; rx < 1280; rx++) {
        if (rx < 112) {
          fb[ry][rx] = 0xFF4A2100; // -I
        } else if (rx < 448) {
          fb[ry][rx] = 0xFFFFFFFF; // white
        } else if (rx < 672) {
          fb[ry][rx] = 0xFF6A0032; // Q
        } else if (rx < 900) {
          fb[ry][rx] = 0xFF131313; // Black
        } else if (rx < 960) {
          fb[ry][rx] = 0xFF000000; // -4
        } else if (rx < 1020) {
          fb[ry][rx] = 0xFF131313; // 0
        } else if (rx < 1080) {
          fb[ry][rx] = 0xFF262626; // +4
        } else if (rx < 1280) {
          fb[ry][rx] = 0xFF131313; // Black
        }
      }
    }
  }
}

void main(void) {
  fb_init(1280, 960, 4, FB_SINGLEBUFFER);

  cbars((unsigned *)fb_get_front_buffer(), fb_get_width(), fb_get_height());

  while (1) ;
}

