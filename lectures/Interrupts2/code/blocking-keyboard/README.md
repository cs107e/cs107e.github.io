Application that expects a PS2 keyboard whose clock line is plugged into GPIO pin
23 and whose data line is plugged into pin 24. Using a blocking interface,
reads PS2 scan codes and converts them into ASCII characters, which it displays
on the screen. The screen has three rows: the top is a ! to show the display
is working. The second shows the most recently typed character. The third shows
a string of recently typed characters.

