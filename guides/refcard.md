---
title: "Guide: Reference Card"
---

Your [course parts kit](/guides/bom) contains a printed copy of the reference card, custom designed by Anna Mistele.

The front side has the pinout for the GPIO pins, you'll be referring to this often! The back side includes pro-tips from our past students. Let us know if you have suggesions of what we should include going forward!

![ref card front](../images/refcard-front.pdf)![ref card back](../images/refcard-back.png)
{: .w-50 .mx-auto .border .p-2 }

Bonus: try the command `pinout.py` for an Ascii pinout when your ref card is not on hand.

```console
$ pinout.py

   |OTG|  |USB|                | HDMI |
O--|   |--|   |----------------| mini |---O
|                  +-------+      +-----+ |
|     Mango Pi     |  D1   |      |micro| |
|      MQ-Pro      |  SoC  |      | sd  | |
|                  +-------+      +-----+ |
|                                         |
| @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ 1 |
| - - - - - - - - - - - - - - - - - - - - |
| @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ |
O-----------------------------------------O

  3V3     1| 2    5V
  PG13    3| 4    5V
  PG12    5| 6    GND
  PB7     7| 8    PB8 (TX)
  GND     9|10    PB9 (RX)
  PD21   11|12    PB5
  PD22   13|14    GND
  PB0    15|16    PB1
  3V3    17|18    PD14
  PD12   19|20    GND
  PD13   21|22    PC1
  PD11   23|24    PD10
  GND    25|26    PD15
  PE17   27|28    PE16
  PB10   29|30    GND
  PB11   31|32    PC0
  PB12   33|34    GND
  PB6    35|36    PB2
  PD17   37|38    PB3
  GND    39|40    PB4
```
