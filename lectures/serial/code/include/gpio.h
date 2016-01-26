#ifndef _GPIO_H_
#define _GPIO_H_

#define GPIO_MAX 54

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

extern void gpio_set_function( unsigned pin, unsigned func );

enum {
  GPIO_PUD_DISABLE  = 0,
  GPIO_PUD_PULLDOWN = 1,
  GPIO_PUD_PULLUP   = 2,
};
extern void gpio_set_pud( unsigned pin, unsigned pud );
extern void gpio_set_pull_up( unsigned pin );
extern void gpio_set_pull_down( unsigned pin );

enum {
  GPIO_RISING_EDGE        = 0,
  GPIO_FALLING_EDGE       = 1,
  GPIO_ASYNC_RISING_EDGE  = 2,
  GPIO_ASYNC_FALLING_EDGE = 3,
  GPIO_HIGH_LEVEL         = 4,
  GPIO_LOW_LEVEL          = 5
};
extern void gpio_detect_falling_edge(unsigned pin);
extern void gpio_detect_rising_edge(unsigned pin);
extern void gpio_detect_async_rising_edge(unsigned pin);
extern void gpio_detect_async_falling_edge(unsigned pin);
extern void gpio_detect_high_level(unsigned pin);
extern void gpio_detect_low_level(unsigned pin);
extern unsigned gpio_check_event(unsigned pin);
extern unsigned gpio_check_and_clear_event(unsigned pin);

extern void gpio_write( unsigned pin, unsigned val );
extern unsigned gpio_read( unsigned pin );

#define GPIO_TX 14
#define GPIO_RX 15

#endif
