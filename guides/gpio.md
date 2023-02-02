---
title: "Guide: Interacting with GPIO pins"
toc: true
quarterprefix: winter23
---

_Written by Anna Mistele_

There are 54 GPIO pins (pins 0 to 53) that we can use in different ways. In this class, we'll be using the pins for **input** and **output**. 
GPIO pins are peripherals, and they are controlled via special memory locations called **peripheral registers.** Keep in mind that peripheral 
registers are not the same as the other registers we learned about (r0–r15).

There are four types of peripheral registers that we'll talk about in 107E: **FSEL**,  **SET**, **CLR**, and **LEV**. Here is a table showing
the addresses of all these registers! (Some of the addresses are Reserved, which means we don't want to mess with them.)

![Table of GPIO registers and their addresses, field names, which pins they address, and whether they are read or write access.
The FSEL addresses (FSEL 0 through FSEL 5) start at 0x20200000 and end at 0x20200014. Each FSEL register covers 10 GPIO pins, so
FSEL 0 maps to gpio pins 0 through 9, FSEL 1 maps to gpio pins 10-19, etc. We have read and write access to the FSEL registers.
Between FSEL and SET registers, the address 0x20200018 is reserved. The SET registers are 0x2020001C (pins 0-31) and 0x20200020
(pins 32-53). The SET registers only have write access. The two CLR registers are the same, at 0x20200028 and 0x2020002C.
The two LEV registers are the same as SET and CLR but they are read-only. They are at 0x20200034 and 0x20200038.](guides/images/gpio-registers.png)

I adapted this table from the [Broadcom BCM2835 ARM Peripherals datasheet, p.90](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)!

Notice that there are a lot more FSEL registers than SET/CLR/LEV. Why does each FSEL register only cover 10 GPIO pins, when the other registers
can cover 32 pins? SET, CLR, and LEV all deal with pins being on or off, which can be represented with just one bit. FSEL, however, sets the function 
of a pin, and a pin can have eight different functions (input, output, ALT0-ALT5), so we need three bits per pin. With each pin taking up three bits,
we can fit 10 pins (30 bits) max into a 32-bit FSEL register. On the other hand, SET/CLR/LEV can hold 32 pins (32 bits). So that's why we only need SET0 
and SET1, for instance, but the function select registers go up to FSEL5.

## FSEL

FSEL registers let us set a GPIO pin's function. As described above, each function is represented as three bits. Input is `000` and output is `001` (the 
other numbers are for alternative functions, which we won't be using in 107E). 

If a pin is set up for **output** (`001`), we can change its value using **SET** and **CLR**. This lets us turn on LEDs, for example.

If a pin is set up for **input** (`000`), we can read its value using LEV. This lets us read whether a button has been pressed, for example.

So if we wanted to set up GPIO pin 3 for output, for instance, we could set bits 6-8 (highlighted below) to `001` with a `str` command in assembly.

![A map of how the bits are utilized in an FSEL register. Each pin consists of three bits, and the two most significant bits are unused.](guides/images/gpio-fsel.png)

## SET

SET registers let us **turn on** a GPIO output pin. (We cannot turn off a GPIO output pin with SET). SET registers are **write-only** (as listed in the table above), 
so we can't read the SET register to figure out if a pin is on or not. (Technically, it will let you read the value, but you can't trust that the values will be accurate.) 

![A map of how the bits are utilized in a SET register. Each pin consists of one bit, and all 32 bits are used.](guides/images/gpio-set.png)

For example, if we wanted to turn on GPIO pin 26, we could write the number `0b00000100000000000000000000000000` into the SET0 register. That will turn on pin 26, 
and it won't turn off any of the other pins (we would need to use a CLR register to turn off pins).

## CLR

CLR registers let us **turn off** a GPIO output pin. Like SET, CLR registers are **write-only**. Writing a `1` to a specific pin in the CLR register will turn off 
the pin. It is not intuitive—you would assume that you write a `1` to turn something on, and a `0` to turn something off! But since we have separate SET and CLR 
registers, we always write a 1, and whether the pin turns on or off is determined by whether we are writing into the SET register or the CLR register.

As shown below, writing `0b00000100000000000000000000000000` into the CLR0 register would turn off GPIO pin 26. Same number as turning on pin 26! Just a different 
address that we write to (`0x20200028`, CLR0, instead of `0x2020001C`, SET0).

![A map of how the bits are utilized in a CLR register. Each pin consists of one bit, and all 32 bits are used.](guides/images/gpio-clr.png)

## LEV

LEV registers let us read the on-off value of a GPIO pin. LEV registers are **read-only**, so we can't write to the LEV register to turn on or off a pin (we use SET 
and CLR instead). We can get the LEV value of a pin whether it is an input or an output pin—besides for debugging purposes, though, it makes most sense to read the 
LEV of an input pin, because we haven't set that value ourselves (it's from an outside source). LEV registers are set up just like SET and CLR registers (32 bits, 
representing 32 pins). If a pin value is `1`, it is on. If a pin value is `0`, it is off.

