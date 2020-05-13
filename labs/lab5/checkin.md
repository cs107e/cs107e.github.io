---
permalink: /labs/lab5/checkin/
title: 'Lab 5: Check-in'
---

During lab, we encourage you to check your understanding with a TA
after completing a section or two, while the material is still fresh
in your mind.

Each question below is a diagnostic question for each section of the
lab. Try answering one after finishing each section to test your
understanding.

1. Back in lab1, we estimated how many instructions the Pi was executing (~20 million/second).  Today you measured the timing of PS/2 clock cycle.  Calculate how many instructions the Pi can execute in that time. Now consider a call to `printf`. Make a ballpark estimate of how many instructions must execute to process and output a single char, now multiply by length of the format string for rough total count. If your keyboard driver makes a call to `printf` in between reading bits that overruns your budget, what will be the consequence?

2. What sequence of codes is sent when typing capital `A`?

3. In a PS/2 scancode, does the least significant or most significant data bit arrive first?

4. Show off that your implementation of `keyboard_read_scancode` correctly receives scancodes from your keyboard. 

5. Confirm that we properly noted your keyboard id in our records.  You may take the keyboard with you, it is  on loan to you for rest of quarter. Please take care of it and return it at the end of quarter.

Before leaving the lab, check in with a TA and discuss any challenges
to answering the questions in the lab. If you do not feel confident
about your responses, please ask us about it too!
