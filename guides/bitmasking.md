---
title: "Guide: Bitmasking"
toc: true
attribution: Written by Anna Mistele
---


What happens when you have a number `x` and you want to AND or OR it with a bit? It actually follows an interesting 
pattern, shown in the table below. Take a moment to convince yourself that this table is correct.

![Chart showing how 1s and 0s AND and OR with a bit x. If you AND a bit with 0, it will always be 0. If you AND a bit
with 1, it will remain the same. If you OR a bit with 0, it will remain the same. If you OR a bit with 1, it will
always be 1.](../images/and-or-chart.png){: .w-50 .zoom}

Alright, so let's imagine we have four bytes of information and we want to change 3 bits.
How would we set the three rightmost bits to `101`?

![00000000000000000000000000000000 OR 101 = 00000000000000000000000000000101](../images/bitmasking1.png){: .w-75 .zoom }

Okay, so it looks like we can set bits just be OR'ing them in. Right?

**WRONG!** What if the starting value isn't all zeroes?

![00000000000000000000000000000111 OR 101 = 00000000000000000000000000000111](../images/bitmasking2.png){: .w-75 .zoom}

That doesn't look quite right. If the existing bit is set to `1`, using an OR won't change it
it stays at `1` (why? check the table above!). To set a bit, we OR it with `1` and to clear a bit, we AND it with `0`  (see the table above!).  In order to change a bit, we would first clear it, then set it.
We will AND with zeroes to "erase" the previous value, but we don't want to erase the whole number.
How do we do this?

![101111001110101110100010101010111 & 111111111111111111111111111111000 = 101111001110101110100010101010000. The important
takeaway here is that if you make a bit string with a bunch of 1s but 0s where you want to clear out a value, then you AND
it with an existing string, it will "mask" / erase the values where there were zeroes in your constructed string.](../images/bitmasking3.png){: .w-75 .zoom}

We create a _bitmask_ that selectively erases only the bits we want to change!
If a bit in our bitmask is set to `1`, it will leave that bit alone. If our bitmask has a `0`, though, it will erase the bit
when we AND our bitmask with the value.

![101111001110101110100010101010111 & 111111111111111111111111111111000 = 101111001110101110100010101010000 OR 101 = 101111001110101110100010101010101.
That's a lot of long numbers, and hard to listen to out loud, but the important thing is that when we use the AND trick we did in the
picture/alt text above, and *then* we OR in a `101`, it will actually put `101` into our string.](../images/bitmasking4.png){: .w-75 .zoom}

Whoaa!! Looks like if we take our value, AND it with a bitmask, and then OR it with the number we want to plug in, we'll have inserted those bits without messing up any of the other bits.

The only issue: No one wants to write out `unsigned int bitmask = 0b111111111111111111111111111111000;` That's a lot of ones. 
Instead, we can write `~0b111`. The tilde (`~`) means take the number `0b111` and flip all the bits (all the trailing zeroes 
turn to `1`s, the three ones turn to `0`s)! So `~0b111` is equivalent to `0b111111111111111111111111111111000`.

Okay there's actually one more issue. What if we want to replace bits in the middle of a value? Well, we can shift our bits to the desired
location. Look at the example bitmask below and figure out which bits are being changed.

```c
// original bits of reg preserved, only 4 targeted bits will change
unsigned int reg = <...>;
reg &= ~(0b1111 << 5);
reg |= (0b1010 << 5);
```
