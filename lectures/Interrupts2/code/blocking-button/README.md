Application that expects a button on GPIO pin 21 that connects pin 21 to ground.
Uses a pull-up resistor, so a button press results in a falling edge. 
Displays the number of button presses on the HDMI screen. 
Since screen refresh is slow, pressing the button several times quickly will
result in lost presses -- motivates interrupts.
