/*
* Use i2c to haptic driver vibration disk motor
* Controller DRV2605, Adafruit module
*
* https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout/downloads
*/
#include "assert.h"
#include "i2c.h"
#include "timer.h"

enum DRV2605_register {
    STATUS = 0x0,
    MODE = 0x01,
    WAVEFORM = 0x04,
    GO = 0x0C,
};

static i2c_device_t *haptic_init(void) {
    const uint8_t addr = 0x5a; // confirm device addr with datasheet, components can differ!
    i2c_device_t *dev = i2c_new(addr);
    const uint8_t DEVICE_ID = 0b111;
    assert((i2c_read_reg(dev, STATUS) >> 5) == DEVICE_ID);
    i2c_write_reg(dev, MODE, 0);
    return dev;
}

static void play_pattern(i2c_device_t *dev, uint8_t *pattern) {
    int psize = 8;
    i2c_write_reg_n(dev, WAVEFORM, pattern, psize);
    uint8_t byte = i2c_read_reg(dev, GO);
    byte |= 0x1;
    i2c_write_reg(dev, GO, byte);
    while (i2c_read_reg(dev, GO) & 1 ) ; // wait until finished
    timer_delay(1);
}

void main(void) {
    i2c_init();
    i2c_device_t *haptic = haptic_init();

    uint8_t hum[8] = {64,64};
    uint8_t click[8] = {10,10,10,10,10,10,10,10};
    uint8_t ramp[8] = {70};
    uint8_t buzz[8] = {47,47,47,47,47,47,47};

    while (1) {
        play_pattern(haptic, hum);
        play_pattern(haptic, click);
        play_pattern(haptic, ramp);
        play_pattern(haptic, buzz);
    }
}