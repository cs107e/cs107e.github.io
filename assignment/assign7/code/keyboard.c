#include "keyboard.h"
#include "system.h"

/* Don't forget to mark any variables touched by interrupts as volatile! */

/* This file should define an interrupt handler for when there is a 
   falling edge on GPIO pin 23. It should read GPIO pin 24 in that
   handler to read in a bit of a PS2 scan code. If it successfully
   receives a scan code, the code should be put into a ring buffer.
   A call to keyboard_has_char() removes scan codes from the ring
   buffer until no remain (returning 0) or a character is read
   (storing the character and returning 1). */

#define INTERRUPT_ENABLE_1  0x2000b210
#define INTERRUPT_ENABLE_2  0x2000b214
#define INTERRUPT_DISABLE_1 0x2000b21c
#define INTERRUPT_DISABLE_2 0x2000b220

#define GPLEV0              0x20200034
#define GPLEV1              0x20200038
#define GPEDS0              0x20200040
#define GPEDS1              0x20200044

/* Initialize the keyboard driver.
   This should set up the keyboard GPIO pins and enable GPIO interrupts
   for them. It should not enable global interrupts: leave that to
   your main program. */
void keyboard_init() {
  
  PUT32(INTERRUPT_DISABLE_1, 0xffffffff);
  PUT32(INTERRUPT_DISABLE_2, 0xffffffff);

  // Put code to configure which GPIO events to trigger interrupts below

  
  // Bit 52 in IRQ registers enables/disables all GPIO interrupts
  // Bit 52 is in the second register, so subtract 32 for index
  PUT32(INTERRUPT_ENABLE_2, (1 << (52 - 32))); 
}


/* Return 1 if there are characters to read, 0 if no characters to read.
   This call should process PS2 scan codes in the ring buffer, returning
   after the first scan code that results in a pressed character. This
   keeps the interrupt handler simple (just put scan codes in ring
   buffer) and puts the complex logic (handling shift, caps lock, etc.)
   in this call.

   This call should not assume that it is followed by keyboard_read_char().
   That is, if a program calls keyboard_has_chars() twice, this should
   not result in characters being lost.*/
unsigned keyboard_has_char() {return 0;}

/* Return the next ASCII character pressed. If keyboard_has_char() is
   false, the return value of this function is undefined. This
   function can assume that keyboard_has_char() was called before it
   and that call returned true. This call does not block. */
char keyboard_read_char() {return '*';}

