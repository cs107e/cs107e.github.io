/* GPIO (general purpose input/output) pins can be configured in many
   ways, so there are a lot of configuration data data register for them.
   Currently this library only supports basic input/output. It does not
   support edge detection, level detection, or pull-up/pull-down
   configuration.
   
   The BCM2835 supports 54 GPIO pins. Each pin can be configured to be
   in one of up to 8 states: input, output, then one of 6 functions
   which is pin-specific. Consult the BCM2835 ARM peripheral manual,
   section 6.2 (pages 102-103) for details on these 6 functions. Some
   pins support 6, some support 3, some actually support none (pins
   46-54).

   In addition to these configurations, a pin can also be configured
   to be pull-up or pull-down. This is useful for when you have an input
   pin. If there is no input, a pull-up pin will default to 1, while
   a pull-down pin will default to 0. The table on pages 102-103 suggests
   each pin can be put into only one of these states. 
*/

#include "gpio.h"
#include "timer.h"
#include "system.h"

#define PULLDOWN 1
#define PULLUP   2

#define GPFSEL0 0x20200000
#define GPFSEL1 0x20200004
#define GPFSEL2 0x20200008
#define GPFSEL3 0x2020000c
#define GPFSEL4 0x20200010
#define GPFSEL5 0x20200014

// Pin output set: Set an output pin to be 1
#define GPSET0  0x2020001C
#define GPSET1  0x20200020
// Pin output clear: Set an output pin to be 0
#define GPCLR0  0x20200028
#define GPCLR1  0x2020002C
// Pin level: read a pin (high or low)
#define GPLEV0  0x20200034
#define GPLEV1  0x20200038
// Pin event detect status (has the event occured)
#define GPEDS0  0x20200040
#define GPEDS1  0x20200044
// Pin rising edge detect 
#define GPREN0  0x2020004C
#define GPREN1  0x20200050
// Pin falling edge detect 
#define GPFEN0  0x20200058
#define GPFEN1  0x2020005C
// Pin high detect
#define GPHEN0  0x20200064
#define GPHEN1  0x20200068
// Pin low detect
#define GPLEN0  0x20200070
#define GPLEN1  0x20200074
// Pin async rising edge detect
#define GPAREN0 0x2020007C
#define GPAREN1 0x20200080
// Pin async falling edge detect
#define GPAFEN0 0x20200088
#define GPAFEN1 0x2020008C

// Pin pull-up/pull-down enable
#define GPPUD   0x20200094
// Pin pull-up/pull-down enabe clock
#define GPPUDCLK0 0x20200098
#define GPPUDCLK1 0x2020009C

/* The BCM2835 manual (p 91-94) gives the layout for function selection
   bits in configuration registers. There are 54 GPIO pins and 8 possible
   functions for each pin (input, output, one of up to 6 other pin-dependent
   uses). So each pin has 3 bits of configuration. There are 6 configuration
   registers, each stores state for up to 10 pins.*/

// Given a pin number, return the function select register to use
unsigned int gpio_pin_to_function_register(unsigned int pin) {
  unsigned int val = pin / 10;
  switch (val) {
  case 0:
    return GPFSEL0;
  case 1:
    return GPFSEL1;
  case 2:
    return GPFSEL2;
  case 3:
    return GPFSEL3;
  case 4:
    return GPFSEL4;
  case 5:
    return GPFSEL5;
  default:
    return 0;
  }
}

// Given a pin number, return the bit offset within its select
// register to use. E.g., if this returns 6 the pin uses bits 6-8.
unsigned int gpio_pin_to_function_offset(unsigned int pin) {
  unsigned int offset = (pin % 10) * 3;
  return offset;
}

unsigned int gpio_pin_to_input_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPLEV0;
  } else {
    return GPLEV1;
  }
}

unsigned int gpio_pin_to_bit_offset(pin) {
  return pin % 32;
}

unsigned int gpio_pin_to_input_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}

unsigned int gpio_pin_to_set_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPSET0;
  } else {
    return GPSET1;
  }
}

unsigned int gpio_pin_to_set_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}

unsigned int gpio_pin_to_clear_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPCLR0;
  } else {
    return GPCLR1;
  }
}

unsigned int gpio_pin_to_clear_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}

unsigned int gpio_pin_to_event_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPEDS0;
  } else {
    return GPEDS1;
  }
}

unsigned int gpio_pin_to_event_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}


unsigned int gpio_pin_to_falling_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPFEN0;
  } else {
    return GPFEN1;
  }
}

unsigned int gpio_pin_to_falling_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}

unsigned int gpio_pin_to_rising_register(unsigned int pin) {
  if (pin <= GPIO_PIN31) {
    return GPREN0;
  } else {
    return GPREN1;
  }
}

unsigned int gpio_pin_to_rising_offset(unsigned int pin) {
  return gpio_pin_to_bit_offset(pin);
}


void gpio_init() {
  timer_init();
}

unsigned int gpio_pin_valid(unsigned int pin) {
  return (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST);
}

unsigned int gpio_function_valid(unsigned int f) {
  return (f >= GPIO_FUNC_INPUT && f <= GPIO_FUNC_ALT5);
}
 
unsigned int gpio_is_input(unsigned int pin) {
  return gpio_get_function(pin) == GPIO_FUNC_INPUT;
}

unsigned int gpio_is_output(unsigned int pin) {
  return gpio_get_function(pin) == GPIO_FUNC_OUTPUT;
}

unsigned int gpio_get_function(unsigned int pin) {
  if (!gpio_pin_valid(pin)) {return GPIO_FUNC_INVALID;}
  system_memory_read_barrier();
  
  unsigned int reg = gpio_pin_to_function_register(pin);
  unsigned int val = GET32(reg);
  val = val >> gpio_pin_to_function_offset(pin);
  val &= 0x7;
  return val;
}

unsigned int gpio_set_input(unsigned int pin) {
  return gpio_set_function(pin, GPIO_FUNC_INPUT);
}

unsigned int gpio_set_output(unsigned int pin) {
  return gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

unsigned int gpio_set_function(unsigned int pin, unsigned int function) {
  if (!gpio_pin_valid(pin)) {return GPIO_FUNC_INVALID;}
  if (!gpio_function_valid(function)) {return GPIO_FUNC_INVALID;}

  system_memory_read_barrier();
  unsigned int reg = gpio_pin_to_function_register(pin);
  unsigned int offset = gpio_pin_to_function_offset(pin);
  unsigned int val = GET32(reg);

  // Remove the original configuration bits
  unsigned int mask = ~(0x7 << offset);
  val = val & mask;

  // Add the new configuration bits
  val |= function << offset;

  // Set the register
  PUT32(reg, val);
  system_memory_write_barrier();
  return function;
}
                      
unsigned int gpio_pin_read(unsigned int pin) {
  unsigned int reg = gpio_pin_to_input_register(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val = val >> gpio_pin_to_input_offset(pin);
  val = val & 1;
  return val;
}

void gpio_pin_write(unsigned int pin, unsigned int boolean) {
  unsigned int reg;
  unsigned int offset;
  
  if (boolean) {
    reg = gpio_pin_to_set_register(pin);
    offset = gpio_pin_to_set_offset(pin);
  } else {
    reg = gpio_pin_to_clear_register(pin);
    offset = gpio_pin_to_clear_offset(pin);
  }

  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

void gpio_detect_falling_edge(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_falling_register(pin);
  offset = gpio_pin_to_falling_offset(pin);

  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

void gpio_detect_rising_edge(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_rising_register(pin);
  offset = gpio_pin_to_rising_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  val |= 1 << offset;
  PUT32(reg, val);
  system_memory_write_barrier();
}

unsigned int gpio_check_event(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_event_register(pin);
  offset = gpio_pin_to_event_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  return (val & (1 << offset));
}

unsigned int gpio_check_and_clear_event(unsigned int pin) {
  unsigned int reg;
  unsigned int offset;

  reg = gpio_pin_to_event_register(pin);
  offset = gpio_pin_to_event_offset(pin);
  system_memory_read_barrier();
  unsigned int val = GET32(reg);
  unsigned int mask = (1 << offset);
  PUT32(reg, mask); // Writing a 1 clears the bit
  system_memory_write_barrier();
  return (val & mask);
}

// These functions are a little complex, they require some
// timing and a busy loop for ~150 cycles. So do them later.

unsigned int gpio_pin_to_udclock_register(unsigned int pin) {
  if (pin <= 31) {
    return GPPUDCLK0;
  } else {
    return GPPUDCLK1;
  }
}
unsigned int gpio_pin_to_udclock_offset(unsigned int pin) {
  return pin & 0x1f;
}

void pin_pull(unsigned int pin, unsigned int pud) {
  PUT32(GPPUD, pud & 3 );
  timer_wait_for(5);
  
  unsigned int reg = gpio_pin_to_udclock_register(pin);
  unsigned int offset = gpio_pin_to_udclock_offset(pin);
  PUT32(reg, 1 << (offset));
  timer_wait_for(5);
  
  PUT32(GPPUD, 0);
  timer_wait_for(5);
  
  PUT32(GPPUDCLK0, 0);
  timer_wait_for(5);
  system_memory_write_barrier();
}

void gpio_set_pullup(unsigned int pin) {
  pin_pull(pin, PULLUP);
}

void gpio_set_pulldown(unsigned int pin) {
  pin_pull(pin, PULLDOWN);
}
/*
 * Copyright (c) 2014 Stanford University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Stanford University nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL STANFORD
 * UNIVERSITY OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Date: 9/26/2014
 */
