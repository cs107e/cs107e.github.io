## Computer Systems from the Ground Up

### Announcements

Welcome to the Winter 2015 offering of CS107e.

*Review the material in [electricity](guides/electricity.md), [binary and hexadecimal numbers](guides/numbers.md), and the [unix command line](guides/unix.md). Test your knowledge by answering the questions in the introduction to each topic.*

*The CS107 Unix help sessions are now posted to the schedule here:*

    http://web.stanford.edu/class/cs107/officehours.html

*Any CS107e student may attend any of the CS107 help sessions. There is no sign-up, just drop in. They are all held in Gates B08.*

    http://piazza.com/stanford/winter2015/cs107e


### Course Information 

**Course Description:** 

CS107 is the third course in Stanford's introductory programming sequence.
CS106 provides students 
with a solid foundation in programming methodology and abstractions,
and CS107 follows on to give you 
the skills needed to build computer systems.
The course will start with the microprocessor and move
up to the C programming language, without skipping anything in between.
The goal is to build a solid understanding of all aspects of
how modern computers execute programs 
and how program development tools work.

Topics covered include:
the ARM architecture, assembly language, and machine-level code;
the C programming language;
compilation, linking and loading; debugging;
networking; memory organization and management;
and controlling peripherals 
such as general-purpose IO pins, graphics, sound, and keyboards.

CS107e is an experimental approach 
that teaches the fundamental concepts of computer systems 
through bare metal programming on the Raspberry Pi.
Bare metal programming means you will not run an operating
system on the Raspberry Pi and will make minimal use of libraries.
This course also serves as a short introduction to embedded systems.

This class is organized by weeks. Each week consists of two lectures
on Fri and Mon, a lab on Tue evening, and a programming assignment
which is due the following Mon at 12 midnight.
Lecture and lab attendance is mandatory.
There will be a short final project,
but no exams. 

Readings will be assigned from a variety of sources,
including *The C Programming Language*, Kernighan and Ritchie.

**Prerequisite:** 106B or X, and consent of instructor. 

**Lectures:** Mon & Fri 12:50-2:05 pm, Rm 107, Littlefield Center

**Lab:** Tue 7:00-9:00 pm, Gates 415

**Lecturers:** Dawson Engler, Pat Hanrahan, Phil Levis

**CA:** Isabel Bush

**Office Hours:**

- Hanrahan, Mon 2:15 to 3:30 pm, Gates 370
- Levis, Fri 11AM-Noon, Gates 412

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
| Lecture 1 (Mon): Introduction |  Review [electricity](guides/electricity.md), [binary and hexadecimal numbers and bitwire operations](guides/numbers.md), and the [unix command line](guides/unix.md).
| *No Lab* | |
| Lecture 2 (Fri): Introduction to ARM : Blinking an LED | |
| **Week 2 Jan 12** | |
| Lecture 3 (Mon): ARM Assembly and Machine Code | |
| Lab 1 : Setup the Raspberry Pi | |
| Assignment 1 : Larson scanner | |
| Lecture 4 (Fri): Introduction to C | |
| **Week 3 Jan 19** | |
| *Martin Luther King Holiday - No Class* | |
| Lab 2 : Below C Level| |
| Assignment 2 : Clock | |
| Lecture 6 (Fri): Functions | |
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

[Labs](labs.md)

[Assignments and grading](assignments.md)

[Late days](late.md)

[Collaboration policy](collaboration.md)

### Resources

[Readings](readings.md)


