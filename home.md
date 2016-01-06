---
layout: page
title: Home
show_on_index: true
---

### Welcome to the Winter 2016 offering of CS107e!

CS107 is the third course in Stanford's introductory programming sequence.
CS106 provides students with a solid foundation in programming methodology and
abstractions, and CS107 follows on to give you the skills needed to build
computer systems. The course will start with the microprocessor and move up to
the C programming language, without skipping anything in between. The goal is
to build a solid understanding of all aspects of how modern computers execute
programs and how program development tools work.

### Announcements

All class announcements will be posted on
[Piazza](http://piazza.com/stanford/winter2016/cs107e).

### Course Info

  -   **Lectures:** Mon & Fri 1:30-2:50 pm, Rm 160-319
  -   **Lab**: Tue or Wed 7:00-9:00 pm, Gates 325
  -   **Lecturers**: Dawson Engler, Pat Hanrahan, Julie Zelenski
  -   **CAs**: Sergio Benitez, Ngoc Bui, Isabel Bush, Omar Rizwan

### Office Hours

  -   **Hanrahan**: Gates 370
  -   **Engler**: Gates 314
  -   **Zelenski**: TBA
  -   **Sergio**: TBA
  -   **Isabel**: Huang Basement

### Schedule

In the readings, K&R is The C Programming Language (Kernighan and Ritchie), 
and EssentialC is a PDF available via [Stanford's CS
Library](http://cslibrary.stanford.edu/101). 
A digital copy of K&R is available
to Stanford students via 
[Safari Books Online](http://proquest.safaribooksonline.com.ezproxy.stanford.edu/book/programming/c/9780133086249).

Please read the assigned readings before attending lecture and lab.
You should also read the [guides](/guides/) for each week.

Note that the schedule includes a guest lecture, Quinn Dunki, 
tentatively scheduled for Fri Feb 26th,
and a tour of the Computer History Musuem on President's Day Feb 16th.

| Topics | Readings |
|--------+----------|
| **Week 1 Jan 4** |
| Lecture 1 (Mon): Introduction ([slides](lectures/intro/intro.pdf)) | Review [electricity](guides/electricity), [binary and hexadecimal numbers and bitwire operations](guides/numbers), and the [unix command line](guides/unix). |
| _No Lab_ (attend a 107 Unix help session!) |
| Assignment 0: [Choose a lab section and learn Git](assignment/assign0) |
| Lecture 2 (Fri): Introduction to ARM | Start with Baking Pi ([intro](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/introduction.html), [ok01](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/ok01.html), [ok02](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/ok02.html)) and then read about [ARM ASM](http://www.toves.org/books/arm/). |
| **Week 2 Jan 11** |
| Lecture 3 (Mon): ARM Assembly and Machine Code | Read sections 4.1-4.5 from the [ARM ISA](readings/armisa.pdf) |
| Lab 1: Setup the Raspberry Pi |
| Assignment 1: Larson scanner |
| Lecture 4 (Fri): Baremetal programming in C | Brush up on C syntax, data types, operators, control structure, function calls. EssentialC chapters 1, 2, and 4; or K&R 1, 2, 3, 4 and 5.1-5.4\. Skip sections involving characters, strings, io, and standard libraries. Read the about the [history of C](http://cm.bell-labs.com/who/dmr/chist.html) |
| **Week 3 Jan 18** |
| _Martin Luther King Holiday - No Class_ |
| Lab 2: Baremetal programming in C |
| Assignment 2: Clock |
| Lecture 5 (Fri): Functions | Read (or read again) about functions in C (Chapter 4 in K&R, Section 4 in Essential C), Read this nice explanation of [local data and frame pointers](http://thinkingeek.com/2014/05/11/arm-assembler-raspberry-pi-chapter-18/) |
| **Week 4 Jan 25** |
| Lecture 6 (Mon): Serial communication, ASCII and strings | Read about characters and strings, basic IO (getc, putc, puts, printf), and structures (Section 1.5, 1.6, 1.9, 5.5, 6, 7 in K&R; Section 3 in EssentialC). Read about [Serial Communication](https://learn.sparkfun.com/tutorials/serial-communication/all). |
| Lab 3: Below C Level |
| Assignment 3: String formatting and printing |
| Lecture 7 (Fri): Linking, loading and starting | Read David Welch's articles on [baremetal programming](https://github.com/dwelch67/raspberrypi/tree/master/baremetal) and [bss data](https://github.com/dwelch67/raspberrypi/tree/master/bssdata). |
| **Week 5 Feb 1** |
| Lecture 8 (Mon): Memory allocation and Valgrind |
| Lab 4: Understanding linking and loading |
| Assignment 4 |
| Lecture 9 (Fri): Graphics and the framebuffer |
| **Week 6 Feb 8** |
| Lecture 10 (Mon): Performance tuning |
| Lab 5: Setting up the framebuffer |
| Assignment 5: Displaying text |
| Lecture 11 (Fri): PS2 Keyboards |
| **Week 7 Feb 15** |
| _President's Day - Computer History Musueum Tour_ |
| Lab 6: PS2 keyboard hack |
| Assignment 6: Keyboard to console |
| Lecture 12 (Fri): Interrupts |
| **Week 8 Feb 21** |
| Lecture 13 (Mon): Interrupts, cont. |
| Lab 7: Handling Interrupts |
| Assignment 7: Full console |
| (Fri): Guest Lecture: Quinn Dunki - Veronica |
| **Week 9 Feb 29** |
| Lecture 14 (Mon): Sound and MIDI |
| Lecture 15 (Fri): Signed and unsigned arithmetic |
| **Week 10 Mar 7** |
| Lecture 16 (Mon) |
| Lecture 17 (Fri): To Linux and Beyond |
{: .schedule}
