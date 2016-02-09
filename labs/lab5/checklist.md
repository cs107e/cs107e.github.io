---
layout: page
title: 'Lab 5: Framebuffer Checklist'
permalink: /labs/lab5/checklist/
---

#### Mailbox

1. Why do we need to check whether the mailbox is EMPTY or FULL?

2. Why is the declaration for `fb_t fb` tagged as `volatile`?

3. Why is the declaration of `mailbox_t mailbox` also tagged as volatile?

4. Why can we add the `addr` and `channel` in `mailbox_write`?
   Could we also `|` them together?
   What bit positions are used for the `addr` and what for the `channel`?

5. What is the purpose of GPU_NOCACHE?

#### Pointers

What is the difference between the following two lines of code?

   char *a = "Hello, world\n";

   char a[]= "Hello, world\n":

What is the difference between the following two declarations?

        int *p[2];

        int (*c)[2];

#### Fonts

What is the width and height of each character in the font?

