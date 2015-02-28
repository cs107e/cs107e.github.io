## Project

The final assignment is a project of your own choosing. 
The goal of the project is to consolidate 
your knowledge of the key concepts in CS107e,
and to do something creative with your Raspberry Pi.

You have two weeks to complete the project. 
There are three important deliverables:

1. *Project Desciption - Due Tue March 3rd*

  Form a team of 1 to 3 people. 
  Send Isabel an email with who's on your team
  and she will set up a github repository for your project.

  Create a 1 page markdown file containing your project proposal.
  Name the file Proposal.md and push it into the github repository.
  The project proposal should include:

  - Name of the project
  - Team members and their responsibilities
  - Goal of the project
  - Resources needed

  We have a small budget for hardware, 
  so we can reimburse you for parts (within reason).
  If you need to mail order parts, it can take time,
  so collect what you need asap.

  Each team member should be responsible for a clearly
  identifiable part of the project.

2. *Project Demonstration - Mon March 16 from 9:30 to 11:30 am*

  We will have a class demo day for all projects.
  The demos are scheduled during the final exam period.
  Each group should prepare a short demonstration of the project.

3. *Project Code and Writeup - Due Tue March 16 at 11:59pm*

  You should be using the github repository for coordinating the
  work amongst your team.
  As such, you should be regularly checking in and out code.
  Do the final check-in by 11:59pm on March 16th.

  Create a short writeup about your project
  and place it in README.md.
  If you have based your project on another public project,
  please describe clearly what parts you used from other people.
  It is ok to use code on the web,
  but you must reference where you got the code,
  and what changes you made.
  It should be clear what you did, and what others did.

  The writeup should also include a short description of what
  work was performed by each member of the team.

## Project Suggestions

The best projects are ones that you think of yourself.
Feel free to browse the internet in search of cool things to do.
Some good sources are [hackaday.com](http://hackaday.com) and 
[adafruit.com/blog](https://blog.adafruit.com).

Two good examples of projects are games and music.
However, there are also lots of other possibilities!

### Game

You have the start of a graphics and input library.
Extend your graphics library to draw additional types of objects,
and your input library to handle a game controller.

Some examples of input devices that you could use to control your game:

- A home-brew game pad made of buttons. Most game pads have
two buttons (A and B) for game control,
and 4 buttons used as a d-pad (left, right, up, down).

- A PS/2 mouse sends codes that are quite similar to the
keyboard scan codes you processed in your previous assignments.
Extend your keyboard code to handle a PS/2 mouse.

- Atari 2600 paddle controllers (or other paddle controllers).

Once you have better input, you are all set to build a game.
Build a simple game such as Pong or Tetris,
or even better a game of your own design.

### Music

We described how to generate notes on the Raspberry Pi.
With that knowledge, you can 

- Create a simple music player that allows the user 
to choose from a list of simple melodies.
Add buttons on a breadboard for control (skip, pause, play, volume?)

- Turn your keyboard into a music keyboard,
so that keys play individual notes and pressing multiple keys 
at the same time combines the notes.
Use a combination of timers and low-level keyboard access 
to enable you to record and then loop tracks.

- Take a music synthesizer such as the Korg Monotron 
and hack it to add MIDI support with one Pi,
then have a second Pi control it over MIDI.
Alternatively, build your own music synthesizer.

## System Components

Your project should be about building a working system.
However, if you build some support infrastructure,
then you can build a more impressive system.
A good sub-project for a team member,
is to build a component of the system.

### Analog Input

Up to now, we have only read digitial values using GPIO.
The Raspberry Pi, unlike a microcontroller like an Arduino,
cannot read analog values.

Many sensors are analog:
  - Photodetectors
  - Potentiometers
  - Temperature sensors
  - Joysticks

In order to read analog values, you will need to interface an analog
to digitial convertor (ADC) to the Raspberry Pi.  An example of an ADC
is the MCP3008, which can be controlled from the PI using the Serial
Peripheral Interface (SPI) protocol. Write a program that uses some
analog inputs to produce an interesting result, such as a screen
visualization or sound.

### Network (Ambitious)

Build a simple network that allows multiple raspberry pis to communicate
over a shared medium (only one Pi can transmit at a time).
One way to do this is to connect TX and RX pins to a
shared line.

It is also possible to communicate by other means.
For example, you could use sound, light, or radio waves.

Finally, you've seen in class how "digital" lines aren't purely digital.
You could design your circuit to make bits as sharp as possible to see how
fast you can transmit and receive. You might find that sending bits faster
means you'll have to send smaller packets, so optimize for overall data
transfer, not just the bit rate.

### GPU (Ambitous)

The raspberry pi has a very powerful GPU.
In fact, the GPU is much faster than the CPU.
The GPU assembly language is similar to,
but different,
than the CPU assembly language.
It is possible to download code to the GPU using a mailbox,
and then call that code.
Learn about the GPU and build a library for programming the GPU.

Can you think of a computationally intensive task 
that could be off-loaded to the GPU? At the very least, you can clear your
screen buffer faster!



