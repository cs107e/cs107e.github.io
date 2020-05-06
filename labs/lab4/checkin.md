---
permalink: /labs/lab4/checkin/
title: 'Lab 4: Check-in'
---


Each question below is a diagnostic question for a lab section. Test your understanding by answering the corresponding question as you finish an exercise. We encourage you to check in with a TA after
completing a section or two, while the material is still fresh in your mind.

1. Compare the symbol list from `nm linking.o` to `nm linking.elf`. How do the symbol addresses from an object file differ from the addresses in the linked executable?  How does the instruction `bl _cstart` change after linking?

2. On a sheet of paper, with address 0x0 at the bottom of the page and
   0x8000000 at the top, sketch a diagram of the program memory for
   `code/linking`. Include the contents of the stack at the point
   where execution has just entered the `main` function. Use the [memory diagram for simple](../images/stack_abs.html)
   as a guide.

3. Show the TA your group's marked up copy of `bootloader.c` with your added comments.

4. How does the saved `pc` in a function’s stack frame relate to the address of the first instruction for the function? If you take the saved `pc` in the stack frame for `main` and subtract it from the saved `lr` in the stack frame for `diff`, what do you get? Express your answer as a number, then explain conceptually what that computation represents. The [memory diagram for simple](../images/stack_abs.html) is a helpful reference.

5. How should the number of `free` calls relate to the number of `malloc` calls in a correctly-written program that uses heap allocation?

Before leaving the lab, check in with a TA and discuss any challenges to
answering the questions in the lab. If you do not feel confident about your
responses, please ask us about it too!
