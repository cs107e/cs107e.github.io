## Computer Systems from the Ground Up

Welcome to the Winter 2015 offering of CS107e.

<!---
Review the material in [electricity](guides/electricity.md), [binary and hexadecimal numbers](guides/numbers.md), and the [unix command line](guides/unix.md). Test your knowledge by answering the questions in the introduction to each topic.

*The CS107 Unix help sessions are now posted to the schedule here:*

    http://web.stanford.edu/class/cs107/officehours.html

*Any CS107e student may attend any of the CS107 help sessions. There is no sign-up, just drop in. They are all held in Gates B08.*
-->

### Announcements

All class announcements will be posted to 
[Piazza](http://piazza.com/stanford/winter2015/cs107e).

### Course Information 

**Lectures:** Mon & Fri 12:50-2:05 pm, Rm 107, Littlefield Center

**Lab:** Tue 7:00-9:00 pm, Gates 415, if you have trouble getting in call Phil's office at 650-725-9046 (across the hall)

**Lecturers:** Dawson Engler, Pat Hanrahan, Phil Levis

**CA:** Isabel Bush

**Office Hours:**

- Hanrahan, Mon 2:15 to 3:30 pm, Gates 370
- Levis, Fri 11AM-Noon, Gates 412
- Engler, Fri 2:15 to 3:30 Gates 314 or appt
- Isabel, Wed noon-2pm, Gates B28; Thurs 4-6pm, Huang Basement

### Schedule

In the readings, K&R is *The C Programming Language* (Kernighan and Ritchie),
and EssentialC is a PDF available at http://cslibrary.stanford.edu/101. 
A digital copy of K&R is available to Stanford students via [Safari Books Online]
(http://proquest.safaribooksonline.com.ezproxy.stanford.edu/book/programming/c/9780133086249).
Please read the assigned readings before attending lecture and lab.

You should also read the [Guides](guides/README.md) for each week.

Note that the schedule includes a guest lecturer
[Quinn Dunki](http://quinndunki.com/blondihacks/)
on Mon Feb 2nd, 
and a tour of the [Computer History Musuem](http://www.computerhistory.org/)
on President's Day Feb 16th.


| Topics | Readings |
| :----- | :------- |
| **Week 1 Jan 5**| |
| Lecture 1 (Mon): Introduction ([slides](lectures/Intro/intro.pdf)) |  Review [electricity](guides/electricity.md), [binary and hexadecimal numbers and bitwire operations](guides/numbers.md), and the [unix command line](guides/unix.md).
| *No Lab* | |
| Lecture 2 (Fri): Introduction to ARM ([slides](lectures/ARM/arm.pdf)) : Blinking an LED ([code](lectures/ARM/code))| Start with Baking Pi ([intro](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/introduction.html), [ok01](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/ok01.html), [ok02](http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/ok02.html)) and then read about [ARM ASM](http://www.toves.org/books/arm/). |
| **Week 2 Jan 12** | |
| Lecture 3 (Mon): ARM Assembly and Machine Code ([slides](lectures/ASM/asm.pdf)) | Read sections 4.1-4.5 from the [ARM ISA](lectures/ASM/armisa.pdf) |
| Lab 1 : [Setup the Raspberry Pi](labs/lab1/walk.md) | |
| Assignment 1 : [Larson scanner](assignment/assign1/assign1.md) | |
| Lecture 4 (Fri): Introduction to C ([slides](lectures/C/c.pdf)), ([code](lectures/C/code)) | Brush up on C syntax, data types, operators, control structure, function calls. EssentialC chapters 1, 2, and 4; or K&R 1, 2, 3, 4 and 5.1-5.4. Skip sections involving characters, strings, io, and standard libraries. Read the about the [history of C](http://cm.bell-labs.com/who/dmr/chist.html)|
| **Week 3 Jan 19** | |
| *Martin Luther King Holiday - No Class* | |
| Lab 2 : [Below C Level](labs/lab2/lab.md)| |
| Assignment 2 : [Clock](assignment/assign2/assign2.md) | |
| Lecture 6 (Fri): Functions ([slides](lectures/Functions/functions.pdf), [code[(lectures/Functions/code))| Read (or read again) about functions in C (Chapter 4 in K&R, Section 4 in Essential C), Read this nice explanation of [local data and frame pointers](http://thinkingeek.com/2014/05/11/arm-assembler-raspberry-pi-chapter-18/)|
| **Week 4 Jan 26** | |
| Lecture 7 (Mon): Serial communication, ASCII and strings | |
| Lab 3 : Programming a UART | |
| Assignment 3 : String formatting and printing | |
| Lecture 6 (Fri): Linking, loading, and starting| |
| **Week 5 Feb 2** | |
| Guest Lecture (Mon): Quinn Dunki, Building Veronica | |
| Lab 4 : Understanding linking and loading | |
| Assignment 4 : Debugger | |
| Lecture 7 (Fri): Graphics and the framebuffer | |
| **Week 6 Feb 9** | |
| Lecture 8 (Mon): Keyboards | |
| Lab 5: Setting up the framebuffer and keyboad| |
| Assignment 5: Terminal program | | 
| Lecture 9 (Fri): Interrupts | |
| **Week 7 Feb 16** | |
| *President's Day - Computer History Musueum Tour* | |
| Lab 6: Working with interrupts
| Assignment 6: Interrupt-driven terminal program
| Lecture 10 (Fri): Audio
| **Week 8 Feb 22** | |
| Lecture 11 (Mon): MIDI Input and Peripherals | |
| Lab 7: PWM and sound | |
| Assignment 7: Sound | |
| Lecture 12 (Fri): Networking I | |
| **Week 9 Mar 2** | |
| Lecture 13 (Mon): Networking II | |
| Lecture 14 (Fri): Memory management: sbrk/malloc | |
| **Week 10 Mar 9** | |
| Lecture 15 (Mon): Caches | |
| Lecture 16 (Fri): To Linux and Beyond | |



### Information

[Course description](course.md)

[Labs](labs.md)

[Assignments and grading](assignments.md)

[Late days](late.md)

[Collaboration policy](collaboration.md)

### Resources

[Readings](readings.md)


