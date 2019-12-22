#include "printf.h"
#include "uart.h"
#include "mcp3008.h"
#include "filters.h"
#include "timer.h"
#include "audio.h"
#include "xcir.h"
void main_printarr(unsigned * arr, unsigned length); 
void main() {
  printf_init();
  uart_init();
  timer_init();
  mcp3008_init();
  filters_init();
  audio_init();
  audio_send_waveform(&filters_reverb_wrapper,50000);
  //audio_send_filter(&filters_phaser);
  //main_printarr(filters_test(&filters_tremolo,10000),10000);
  //filters_printtremarr();
}

void main_printarr(unsigned * arr, unsigned length) {
  for(int i = 0; i < length; i++) printf("%d ",arr[i]);
}
