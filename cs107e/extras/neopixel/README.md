
Datasheet
    https://cdn-shop.adafruit.com/datasheets/WS2812.pdf

Useful blog post
    https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

Neopixel timing is very tight (nanosec accurancy), read blog to get basi
No clock line, single data line. Send zero by holding data line high
for time t (= ~350ns), send one by holding data high 2*t (= ~700ns)
The only truly critical step is keeping width of zero bit short enough to not be interpreted as one bit.

Latch frame by holding data line low >= 55us

