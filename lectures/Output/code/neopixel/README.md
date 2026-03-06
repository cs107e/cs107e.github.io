docs/ folder has
    WS2812 datasheet


Useful blog post
    https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

Neopixel timing is very tight (nanosecond accurancy), read blog post for info
No clock line, serial data line. Send zero by holding data line high
for time t (= ~350ns), send one by holding data high 2*t (= ~700ns)
The only truly critical step is keeping width of zero bit short enough to not be interpreted as one bit.

Latch frame by holding data line low >= 55us
