#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

/* Functions for Raspberry Pi GPIO. */

enum {
  GPIO_PIN_FIRST = 0,
  GPIO_PIN0 = 0,
  GPIO_PIN1 = 1,
  GPIO_PIN2 = 2,
  GPIO_PIN3 = 3,
  GPIO_PIN4 = 4,
  GPIO_PIN5 = 5,
  GPIO_PIN6 = 6,
  GPIO_PIN7 = 7,
  GPIO_PIN8 = 8,
  GPIO_PIN9 = 9,
  GPIO_PIN10 = 10,
  GPIO_PIN11 = 11,
  GPIO_PIN12 = 12,
  GPIO_PIN13 = 13,
  GPIO_PIN14 = 14,
  GPIO_PIN15 = 15,
  GPIO_PIN16 = 16,
  GPIO_PIN17 = 17,
  GPIO_PIN18 = 18,
  GPIO_PIN19 = 19,
  GPIO_PIN20 = 20,
  GPIO_PIN21 = 21,
  GPIO_PIN22 = 22,
  GPIO_PIN23 = 23,
  GPIO_PIN24 = 24,
  GPIO_PIN25 = 25,
  GPIO_PIN26 = 26,
  GPIO_PIN27 = 27,
  GPIO_PIN28 = 28,
  GPIO_PIN29 = 29,
  GPIO_PIN30 = 30,
  GPIO_PIN31 = 31,
  GPIO_PIN32 = 32,
  GPIO_PIN33 = 33,
  GPIO_PIN34 = 34,
  GPIO_PIN35 = 35,
  GPIO_PIN36 = 36,
  GPIO_PIN37 = 37,
  GPIO_PIN38 = 38,
  GPIO_PIN39 = 39,
  GPIO_PIN40 = 40,
  GPIO_PIN41 = 41,
  GPIO_PIN42 = 42,
  GPIO_PIN43 = 43,
  GPIO_PIN44 = 44,
  GPIO_PIN45 = 45,
  GPIO_PIN46 = 46,
  GPIO_PIN47 = 47,
  GPIO_PIN48 = 48,
  GPIO_PIN49 = 49,
  GPIO_PIN50 = 50,
  GPIO_PIN51 = 51,
  GPIO_PIN52 = 52,
  GPIO_PIN53 = 53,
  GPIO_PIN54 = 54,
  GPIO_PIN_LAST =  54
};

enum {
  GPIO_FUNC_INPUT   = 0,
  GPIO_FUNC_OUTPUT  = 1,
  GPIO_FUNC_ALT0    = 4,
  GPIO_FUNC_ALT1    = 5,
  GPIO_FUNC_ALT2    = 6,
  GPIO_FUNC_ALT3    = 7,
  GPIO_FUNC_ALT4    = 3,
  GPIO_FUNC_ALT5    = 2,
  GPIO_FUNC_INVALID = 8 // Only returned for invalid pins
};

void gpio_init();
unsigned int gpio_pin_valid(unsigned int pin);
unsigned int gpio_function_valid(unsigned int pin);
unsigned int gpio_is_input(unsigned int pin);
unsigned int gpio_is_output(unsigned int pin);
unsigned int gpio_get_function(unsigned int pin);

unsigned int gpio_set_input(unsigned int pin);
unsigned int gpio_set_output(unsigned int pin);
unsigned int gpio_set_function(unsigned int pin, unsigned int function);
                      
unsigned int gpio_pin_read(unsigned int pin);
void gpio_pin_write(unsigned int pin, unsigned int boolean);

void gpio_set_pullup(unsigned int pin);
void gpio_set_pulldown(unsigned int pin);

void gpio_detect_falling_edge(unsigned int pin);
void gpio_detect_rising_edge(unsigned int pin);

unsigned int gpio_check_event(unsigned int pin);
unsigned int gpio_check_and_clear_event(unsigned int pin);

#endif


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
