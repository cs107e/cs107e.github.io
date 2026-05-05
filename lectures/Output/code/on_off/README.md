docs/ folder contains
    SN74HC595N datasheet


sos
---
Demo program from Serial lecture
It pulses the S-O-S pattern on gpio PB4.
We can connect PB4 to different types of outputs
(led, active buzzer, solenoid, vibrating motor)
and signal our distress is lots of ways!


count
-----
Uses a SIPO (serial in parallel out) shift
register to control 8 outputs with 3 gpio pins
(Data, Shift, Latch)