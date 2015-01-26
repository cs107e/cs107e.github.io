#ifndef GPIO_DEFS
#define GPIO_DEFS

// mode = ...
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2
extern void pinMode( int pin, int mode );

#define PULLDOWN 1
#define PULLUP   2
extern void pinPullUpDown( int pin, int pud );
extern void pinPullUp( int pin );
extern void pinPullDown( int pin );

extern void pinPad( int pin, int current );
extern void pinClock( int frequency );

extern void pinWrite( int pin, int val );
extern int pinRead( int pin );

#endif
