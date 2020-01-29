---
permalink: /labs/lab3/checkin/
title: 'Lab 3: Check-in'
---

1. Explain how the `lr` register is used as part of making a function call. Which instruction writes to the `lr` register? Which instruction reads from it? What commands could you use in `gdb` to observe the changes to the `lr` register during execution of a function call?

2. Why is it necessary to plug in both `TX` and `RX` for loopback mode to work?

3. On a hosted system, executing an incorrect call to `strlen` (e.g. argument is an invalid address or unterminated string) can result in a runtime error (crash/halt). But when running bare metal on the Pi, every call to `strlen` (whether well-formed or not) will complete "successfully" and return a value. Explain the difference in behavior. What is the return value for an erroneous call?

![](https://imgs.xkcd.com/comics/compiler_complaint.png)
