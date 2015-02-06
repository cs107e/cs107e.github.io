#include "video.h"
#include "led.h"

void notmain() {
  led_init();
  video_init();
  
  char* buffer = (char*)video_writeable_buffer();
  int length = video_byte_depth() * video_height() * video_width();
  int i;
  while (1) {
    for (i = 0; i < length; i += 4) {
      buffer[i] = 0xFF;
      buffer[i + 3] = 0xFF;
    }
    
    for (i = 0; i < 1000000; i++) {}
      
    for (i = 0; i < length; i++) {
      buffer[i] = 0x00;
    }
  }
}
