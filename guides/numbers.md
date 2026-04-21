---
title: "Guide: Binary and Hexadecimal Numbers"
---

Everyone is comfortable with decimal numbers expressed in the familiar base 10 system.
However, computers internally use binary (base 2) made up of just zeroes and ones.
To understand how computers represent and manipulate numbers, we'll need to learn about bits and bitwise operators.


Here are some skills to get into your repertoire:
- Conversions
    - List the decimal numbers 0 to 15 and rewrite into binary and hexadecimal equivalents. Know these conversions for small values.
    - Get comfortable translating between hex and binary. Each group of 4 bits = one hex digit.
    - Don't worry about converting between decimal and hex/binary, too tedious to do by hand.
- Learn to read some of common binary patterns
  -  What is the binary pattern for a power of two? What about a number one less than a power of two? How does the bit pattern show whether a number is odd or even?
    - Memorize small powers of 2.
- Know range for integer types
    - e.g. char is 8 bits, 2^8 = 256 different bit patterns. What is range for 16-bit short, 32-bit int, 64-bit long?
- Practice with the bitwise operators
    -  `&` (and) `|` (or) `^` (xor) `~` (invert)
        - write out the truth table for each operation
    - `<<` (shift left) `>>` (shift right)
        - how does the value of the number change with you shift left? shift right?
    - use the bitwise operations to construct expression to set a bit, clear a bit, toggle a bit

Try your hand at these [exercises on base conversion and bitwise operations](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/lab1/practice.html) used for practice in the bits lab of the regular CS107.

Questions to self-test your understanding:

- What is the decimal value of `0b1000`?
- What is `255` in hexadecimal?
- What is `0xA` in binary?
- What is the minimum number of bits required to store the number 8?
- What does `0b010` + `0b111` equal?
- What is the result of evaluating the following expressions:
    - `0x01 & 0x03`
    - `0x01 | 0x12`
    - `0x01 ^ 0x13`
    - `0x01 << 2`
- Given a number `x`, what expression computes a new number `y`
  that is equal to `x` but with a zero in its least significant bit?

If you haven't had much experience with bits and bitwise operators,
here are some materials you may want to review:

* Khan Academy video on [Number Systems](https://www.khanacademy.org/math/pre-algebra/applying-math-reasoning-topic/alternate-number-bases/v/number-systems-introduction)
* Wikipedia entry for [Binary Numbers](http://en.wikipedia.org/wiki/Binary_number)
* Sparkfun tutorial on [Hexadecimal Numbers](https://learn.sparkfun.com/tutorials/hexadecimal)
* Wikipedia entry for [Bitwise Operators](http://en.wikipedia.org/wiki/Bitwise_operations_in_C)
* Play with the [Bitwise Explorer](https://mdrach.github.io/cs107-apps/), an visualizer for bitwise expressions written by CS107 TA Max Drach

------
<small>Credit to: <http://cowbirdsinlove.com/43></small>
![all your base](../images/allyourbase.png)


