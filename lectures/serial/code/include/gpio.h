#ifndef GPIO_DEFS
#define GPIO_DEFS

#define GPIO_MAX 54

// mode = ...
#define GPIO_FUNC_INPUT  0
#define GPIO_FUNC_OUTPUT 1
#define GPIO_FUNC_ALT0   4
#define GPIO_FUNC_ALT1   5
#define GPIO_FUNC_ALT2   6
#define GPIO_FUNC_ALT3   7
#define GPIO_FUNC_ALT4   3
#define GPIO_FUNC_ALT5   2
extern void gpio_set_function( unsigned pin, unsigned func );

#define GPIO_PULLDOWN 1
#define GPIO_PULLUP   2
extern void gpio_pull_updown( unsigned pin, unsigned pud );
extern void gpio_pull_up( unsigned pin );
extern void gpio_pull_down( unsigned pin );

#define GPIO_RISING_EDGE        0
#define GPIO_FALLING_EDGE       1
#define GPIO_ASYNC_RISING_EDGE  2
#define GPIO_ASYNC_FALLING_EDGE 3
#define GPIO_HIGH_LEVEL         4 
#define GPIO_LOW_LEVEL          5
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

extern void gpio_pad( unsigned pin, unsigned current );

extern void gpio_clock( unsigned frequency );

#define GPIO_TX 14
#define GPIO_RX 15

#endif
