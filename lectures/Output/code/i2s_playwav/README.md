## PCM1502 hardware
For the demo I am using a PCM1502 DAC
https://www.amazon.com/dp/B09C5QX228

```
PCM1502     Mango Pi
-------     --------
Vin:        3.3V (5V tolerant?)
Gnd:        Gnd
LRCK:       PB6 (left/right(frame) clock) (Mango I2S2 FS)
BCK:        PB5 (bit clock) (Mango IS2S Clk)
DIN:        PB4 (data) (Mango I2S2 Data Out)
SCK:        Gnd (master clock)
```
Tying SCK to ground tells PCM1502 to generate master clock using its internal PLL (can perma set by solder bridge on top side).
When SCK left floating, behavior is janky.

Underside of PCM1502 has solder pads for four settings either H or L.
May need solder bridges to appropriate settings

- H1L: FLT "low" for normal latency filter
- H2L: DEMP "low" to disable
- H3L: XSMT "high" to disable
- H4L: FMT "low" for I2S

Useful info with photos: https://raspberrypi.stackexchange.com/a/94951

## Convert wav file to PCM sample data
`wav-to-c-header.py` is a python script that reads a .wav file and writes a header file which declares of C array of samples.
#include header file into your program