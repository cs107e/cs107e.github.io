#include "memory.h"
#include "timer.h"
#include "gpio.h"
#include "pwm.h"

// Nominal clock frequencies
#define F_OSC   19200000
#define F_PLLD 500000000

#define BM_PASSWORD 0x5A000000

#define CM_PWMCTL 0x201010A0
#define CM_PWMDIV 0x201010A4

#define CM_ENABLE  (1 << 4)
#define CM_KILL    (1 << 5)
#define CM_BUSY    (1 << 7) 
#define CM_FLIP    (1 << 8)
#define CM_MASH0   (0 << 9)
#define CM_MASH1   (1 << 9)
#define CM_MASH2   (2 << 9)
#define CM_MASH3   (3 << 9)


#define PWM_CTL    0x2020C000
#define PWM_STATUS 0x2020C004
#define PWM_DMAC   0x2020C008
#define PWM_FIFO   0x2020C018

#define PWM0_RANGE 0x2020C010
#define PWM0_DATA  0x2020C014

#define PWM1_RANGE 0x2020C020
#define PWM1_DATA  0x2020C024

#define PWM0_ENABLE      0x0001
#define PWM0_SERIAL      0x0002
#define PWM0_REPEATFF    0x0004
#define PWM0_OFFSTATE    0x0008
#define PWM0_REVPOLARITY 0x0010
#define PWM0_USEFIFO     0x0020
#define PWM0_MARKSPACE   0x0080

#define PWM1_ENABLE      0x0100
#define PWM1_SERIAL      0x0200
#define PWM1_REPEATFF    0x0400
#define PWM1_OFFSTATE    0x0800
#define PWM1_REVPOLARITY 0x1000
#define PWM1_USEFIFO     0x2000
#define PWM1_MARKSPACE   0x8000

#define PWM_CLEARFIFO    (1 << 6)

void pwm_init(void)
{
    //pwm_set_clock( F_OSC );
    //pwm_set_mode( 0, PWM_MARKSPACE );
    //pwm_set_fifo( 0, 0 );
    //pwm_set_mode( 1, PWM_MARKSPACE );
    //pwm_set_fifo( 1, 0 );
}

/*
 * pwm frequency in Hz = 19 200 000 Hz / pwmClock / pwmRange
 * pwmClock is really a divider (e.g., if we want a divider of 16, we
 * should pass in a freq of 19 200 000 / 16)
 */
void pwm_set_clock(int freq) {
    int timer = F_OSC;
    int source;

    // 2^20 = 1048576
    // 2^21 = 2097152
    /* source
    0 = GND
    1 = oscillator
    2 = testdebug0
    3 = testdebug1
    4 = PLLA per
    5 = PLLC per
    6 = PLLD per
    7 = HDMI auxiliary
    8-15 = GND
    */
    if (timer == F_OSC) { // 19 200 000 = 2^11 x 3 * 5^5
        source = 1;
    } else if (timer == F_PLLD) {
        source = 6;
    } else {
        source = 1;
        timer = F_OSC;
    }

    int divisor  = timer / freq; // for freq=16, divisor = 1.2e6
    int fraction = (timer % freq) * 4096 / freq; // fraction = 0
    int mash = fraction ? CM_MASH1 : CM_MASH0;

    if(      mash == CM_MASH0 && divisor < 1 )
        divisor = 1;
    else if( mash == CM_MASH1 && divisor < 2 )
        divisor = 2;
    if (divisor > 4095)
        divisor = 4095;

    if (fraction > 4095)
        fraction = 4095;

    int pwm = GET32(PWM_CTL); // save pwm control register

    // turn off pwm before changing the clock
    PUT32(PWM_CTL, 0);

    PUT32(CM_PWMCTL, BM_PASSWORD) ;          // turn off clock
    while (GET32(CM_PWMCTL) & CM_BUSY) ;     // wait for clock to stop

    PUT32(CM_PWMDIV, BM_PASSWORD | (divisor << 12) | fraction);

    PUT32(CM_PWMCTL, BM_PASSWORD | CM_ENABLE | mash | source);

    PUT32(PWM_CTL, pwm); // restore pwm control register
}


void pwm_enable(int chan) 
{
    int ra = GET32(PWM_CTL);
    if (chan == 0) {
        ra |=  PWM0_ENABLE;
    }
    if (chan == 1) {
        ra |=  PWM1_ENABLE;
    }
    PUT32(PWM_CTL, ra);
}

void pwm_disable(int chan) 
{
    int ra = GET32(PWM_CTL);
    if (chan == 0) {
        ra &=  ~PWM0_ENABLE;
    }
    if (chan == 1) {
        ra &=  ~PWM1_ENABLE;
    }
    PUT32(PWM_CTL, ra);
}


void pwm_set_mode(int chan, int markspace) {
    int ra = GET32(PWM_CTL);
    if (chan == 0) {
        if (markspace) {
            ra |=  PWM0_MARKSPACE;
        } else {
            ra &= ~PWM0_MARKSPACE;
        }
    }
    if (chan == 1) {
        if (markspace) {
            ra |=  PWM1_MARKSPACE;
        } else {
            ra &= ~PWM1_MARKSPACE;
        }
    }
    PUT32(PWM_CTL, ra);
}


void pwm_set_fifo(int chan, int usefifo) {
    int ra = GET32(PWM_CTL);
    if (chan == 0) {
        if (usefifo) {
            ra |=  PWM0_USEFIFO;
        } else {
            ra &= ~PWM0_USEFIFO;
        }
    }
    if (chan == 1) {
        if (usefifo) {
            ra |=  PWM1_USEFIFO;
        } else {
            ra &= ~PWM1_USEFIFO;
        }
    }
    PUT32(PWM_CTL, ra);
}

void pwm_clear_fifo(void)
{
    unsigned ra = GET32(PWM_CTL);
    PUT32(PWM_CTL, ra | PWM_CLEARFIFO);
}

/*
 * range should be between 0 and 4095
 */
void pwm_set_range(int chan, int range) {
    if (chan == 0) {
        PUT32(PWM0_RANGE, range);
    }
    if (chan == 1) {
        PUT32(PWM1_RANGE, range);
    }
}

void pwm_set_width(int chan, int width) {
    if (chan == 0) {
        PUT32(PWM0_DATA, width);
    }
    if (chan == 1) {
        PUT32(PWM1_DATA, width);
    }
}

unsigned pwm_get_status(void) {
    return GET32(PWM_STATUS);
}

void pwm_write(int value) {
    PUT32(PWM_FIFO, value);
}
