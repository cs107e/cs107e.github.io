Demonstrates how to link to cs107e libpi

1. Can't find .h file (gpio.h not found)
 
Remove -I

2. Can't find -lp

Remove -L

3. Multiple definitions of gpio_init

Add gpio_init to main

4. arm-none-eabi-nm $CS107e/lib/libpi.a

