docs/ folder contains
    PCM1502 datasheets (i2s DAC)
    SPH0645LM4H datasheet (i2s microphone)

Playback using PCM1502 DAC
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
Can permanently connect SCK to GND by solder small pad

Underside of PCM1502 has solder pads for four settings either H or L.
We soldered these settings to our board:
    - H1L: FLT "low" for normal latency filter
    - H2L: DEMP "low" to disable
    - H3L: XSMT "high" to disable
    - H4L: FMT "low" for I2S
Useful post with photos: https://raspberrypi.stackexchange.com/a/94951

I2S MEMS microphone SPH0645LM4H
https://www.adafruit.com/product/3421

## Converting wav file to PCM sample data
`wav-to-c-header.py` is a python script that reads a .wav file and outputs array of sample as C header file.
Compile data into your program by #include
