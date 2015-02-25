#include "gfx.h"
#include "gpio.h"
#include "system.h"

#define BASIC_IRQ_PENDING   0x2000b200
#define IRQ_PENDING_1       0x2000b204
#define IRQ_PENDING_2       0x2000b208
#define INTERRUPT_ENABLE_1  0x2000b210
#define INTERRUPT_ENABLE_2  0x2000b214
#define INTERRUPT_DISABLE_1 0x2000b21c
#define INTERRUPT_DISABLE_2 0x2000b220

#define GPLEV0              0x20200034
#define GPLEV1              0x20200038
#define GPEDS0              0x20200040
#define GPEDS1              0x20200044

void int_handler(unsigned int pc) {
  // Your interrupt handler, invoked from handlers.c
}

void notmain() {
  gfx_init();
  gpio_init();

  // BCM2835 manual, section 7.5
  PUT32(INTERRUPT_DISABLE_1, 0xffffffff);
  PUT32(INTERRUPT_DISABLE_2, 0xffffffff);

  // Put code to configure which GPIO events to trigger interrupts below


  // Bit 52 in IRQ registers enables/disables all GPIO interrupts
  // Bit 52 is in the second register, so subtract 32 for index
  PUT32(INTERRUPT_ENABLE_2, (1 << (52 - 32))); 

  // Now GPIO interrupts are enabled, but interrupts are still
  // globally disabled. So enable them.
  system_enable_interrupts();

  while (1) {
    gfx_clear();
    // Draw state of program here
    gfx_draw();
  }
}

