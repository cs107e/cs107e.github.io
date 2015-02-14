#include "gfx.h"
#include "gpio.h"
#include "system.h"

void hexval_to_string(char* buf, char* header, unsigned int hexval);
void display_state();
unsigned int bit_count = 0;
unsigned int interrupt_lr = 0xdeadbeef;

void notmain() {
  gfx_init();
  while (1) {
    gfx_clear();
    display_state();
    gfx_draw();
    bit_count++;
  }
}


void display_state() {
  char str[100];
  int  y = 20;
  unsigned int c;
  unsigned int p; 
  unsigned int l;
  unsigned int s;
  unsigned int f;

  asm volatile (" mrs  %0, cpsr" : "=r" (c) :  );
  hexval_to_string(str, "CPSR:   ", c); 
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  c &= 0x1F;
  switch (c) {
  case 0x10:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   user");
    break;
  case 0x11:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   fast interrupt");
    break;
  case 0x12:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   interrupt");
    break;
  case 0x13:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   supervisor");
    break;
  case 0x17:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   abort");
    break;
  case 0x1b:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   undefined");
    break;
  case 0x1f:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   system");
    break;
  default:
    gfx_draw_string(0xFFFFFFFF, 100, y += 20, "MODE:   UNKNOWN!");
    break;
  }
  asm volatile ("mov %0, PC" : "=r"(p));
  hexval_to_string(str, "PC:     ", p);
  gfx_draw_string(0xFFFFFFFF, 100, y += 40, str);
  asm volatile ("mov %0, LR" : "=r"(l));
  hexval_to_string(str, "LR:     ", l);
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  hexval_to_string(str, "INT_LR  ", interrupt_lr);
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  asm volatile ("mov %0, SP" : "=r"(s));
  hexval_to_string(str, "SP:     ", s);
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  asm volatile ("mov %0, FP" : "=r"(f));
  hexval_to_string(str, "FP:     ", f);
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  hexval_to_string(str, "STR:    ", (unsigned int)str);
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  
  hexval_to_string(str, "GPEDS0: ", GET32(0x20200040)); // GPEDS0
  gfx_draw_string(0xFFFFFFFF, 100, y += 40, str);
  hexval_to_string(str, "GPEDS1: ", GET32(0x20200044)); // GPEDS1
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  hexval_to_string(str, "BIRQP:  ", GET32(0x2000B200)); // Basic IRQ
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  hexval_to_string(str, "IRQP1:  ", GET32(0x2020B204)); // IRQ1
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  hexval_to_string(str, "IRQP2:  ", GET32(0x2020B208)); // IRQ2 
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);
  
  hexval_to_string(str, "GPLEV0: ", GET32(0x20200034)); // GPLEV0
  gfx_draw_string(0xFFFFFFFF, 100, y += 40, str);
  hexval_to_string(str, "GPLEV1: ", GET32(0x20200038)); // GPLEV1
  gfx_draw_string(0xFFFFFFFF, 100, y += 20, str);

  hexval_to_string(str, "COUNT:  ", bit_count);
  gfx_draw_string(0xFFFFFFFF, 100, y += 40, str);
}


char nibble_to_char(unsigned int nibble) {
  switch (nibble) {
  case 0x0:
    return '0';
  case 0x1:
    return '1';
  case 0x2:
    return '2';
  case 0x3:
    return '3';
  case 0x4:
    return '4';
  case 0x5:
    return '5';
  case 0x6:
    return '6';
  case 0x7:
    return '7';
  case 0x8:
    return '8';
  case 0x9:
    return '9';
  case 0xa:
    return 'a';
  case 0xb:
    return 'b';
  case 0xc:
    return 'c';
  case 0xd:
    return 'd';
  case 0xe:
    return 'e';
  case 0xf:
    return 'f';
  default:
    return '!';
  }
}

void hexval_to_string(char* buf, char* header, unsigned int hexval) {
  int i;
  for (i = 0; header[i] != 0; i++) {
    buf[i] = header[i];
  }
  buf[i++] = '0';
  buf[i++] = 'x';
  int j;
  for (j = 7; j >= 0; j--) {
    unsigned int nibble = (hexval >> (j * 4)) & 0xf;
    buf[i++] = nibble_to_char(nibble);
  }
  buf[i] = 0;
}
