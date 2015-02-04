## Lab 4: Linking and Loading

*Lab written by Pat Hanrahan*

### Goals

During this lab you will:

1 Experiment with the linker `ld`,
with the goal of understanding 
what is in an object (ELF) file,
and how object files are combined to produce an executable.

2 Read and understand the source to the bootloader.
It is important to read code produced in the wild.

To complete the lab,
you must answer the questions in the 
[check list](checklist.md).

The second question involves writing a
description of how the bootloader works.
See the detailed instructions in the second section of the lab.


### Lab exercises

1 **Linking**

In the first part of this lab,
you should repeat some of live coding demonstrations
showed during the lecture on linking and loading.

To start, check out the [code](../../lectures/LD/code)
that was used in lecture.

**Makefiles**

Start by looking at the Makefile.
Note the dependency

    main.exe: main.o cstart.o start.o
        $(ARMGNU)-ld $(LDFLAGS) -T memmap $^ -o $@
        $(ARMGNU)-objdump -D $@ > $*.exe.list

Make sure you understand this Makefile. 
The variables beginning with `$` expand into a filename
or a list of filenames.

- What does `$@` expand into?

- What does `$*` expand into?

- What does `$^` expand into?

**Symbols in Object Files**

Type 

    % make clean
    % make start.o
    % arm-none-eabi-nm start.o

What is the purpose of `arm-none-eabi-nm`?
What does it print out?
What do the single letter symbols 'T', 'U', and 't' mean?

    % make main.o
    % arm-none-eabi-nm main.o

What does this command print out? 
What does the print out tell you about the variables
and the functions in `main.c`.

    % make cstart.o
    % arm-none-eabi-nm cstart.o

What does `arm-none-eabi-nm` tell us about the symbols in `cstart.o`.

**Linking Object Files into Executables**

    % make main.exe
    arm-none-eabi-ld  -T memmap main.o cstart.o start.o -o main.exe
    ...

`arm-none-eabi-ld` is the command 
that *links* the three files together to form a single executable.

    % arm-none-eabi-nm -d main.exe

What has happened during the link process?

Let's disassemble `start.o`.

    % arm-none-eabi-objdump -d start.o
    ...
    00000000 <_start>:
       0: e3a0d902    mov sp, #32768  ; 0x8000
       4: ebfffffe    bl  0 <cstart>

Note that branch at location 4.  
The *branch and link* instruction `bl` branches to location 0.
Is 0 the address of `cstart`?

Now let's disassemble `main.exe`, 
and let's look at the same code after linking.

    % arm-none-eabi-objdump -d main.exe
    ...
    00008000 <_start>:
    8000:    e3a0d902    mov sp, #32768  ; 0x8000
    8004:    eb000036    bl  80e4 <cstart>
    ...
    000080e4 <cstart>:
    80e4:    e92d4800    push    {fp, lr}
    80e8:    e28db004    add fp, sp, #4
    80ec:    e24dd018    sub sp, sp, #24
    ...

What is the difference between the branch address 
before and after linking?
What did the linker do to change the address?

**memmap**

When you run 

    % make main.exe
    arm-none-eabi-ld  -T memmap main.o cstart.o start.o -o main.exe
    arm-none-eabi-objdump -D main.exe > main.list

You also generate a listing `main.list` of the executable. 
Look at the listing.
Understand how the `.text`, `.data`, `.rodata`, and `.bss`
sections from the different files have been combined.

Now look at `memmap`. 

- Do you understand how the `memmap` linker script
specifies how sections from individual files are to be combined?

- One of the purposes of `memmap` is to ensure that
the global label `start` appears first in the executable file.
How does `memmap` specify that `start.o` should be first?

- Another purpose of `memmap` is to setup block storage
of uninitialized variables.
It does this by defining the addresses
`__bss_start__` and `__bss_end__`.
How are those addresses determined?
In C, uninitialized variables should be set to 0.
How does `cstart` use those addresses to initialize the variables to 0?


2 **The bootloader**

The second part of the lab involves
reading and understanding
the bootloader code.
The bootloader we are using was written by David Welch,
the person most responsible for figuring out how
to write bare metal programs on the Raspberry Pi.
If it wasn't for his work,
we would not be offering this course.

There is another reason for reading his code;
it is just as important to read other peoples code as your own.
Reading an expert's code will make you a better programmer.
They often use different programming techniques than what you use.
Also, everyone has a different programming style,
and it is instructive to compare styles and
discuss what is good and bad about different styles.

Pull the [bootloader code](code/bootloader) from github.
This is the code that runs on the Raspberry Pi.
It is normally installed on your SD card as `kernel.img`.
When the Pi boots,
it loads the bootloader code,
and starts running it.
Your laptop sends the binary to the Raspberry Pi
using the usb-serial breakout board.
The bootloader uses the Pi's UART to communicate with the host.
It receives the binary,
loads it into memory,
and then branches to the code to begin execution.

First, read the assembly language file `start.s`. 
Note the `.space` directive between `_start` and the label `skip`.
This has the effect of placing the bootloader code 
at location `0x200000`.
This creates a hole in memory
(between 0x8000 and 0x200000).
The bootloader loads your program into that hole.
Why can't the bootloader code also be placed at 0x8000?

Second, Welch provides his own library that
implements GPIO, timers, and the UART.
The code is in `periphs.c`.
Don't focus too much on that code during the lab,
but it is worthwhile reading how he implemented
some of the functions that you have implemented
during the last few weeks.

**XMODEM**

In this course,
we have been using the program rpi-install.py to send 
the program binary to the Raspberry Pi.
The laptop and the Pi communicate using a simple
file transfer protocol called XMODEM.
In the jargon of XMODEM,
the host laptop is called the tranmitter.

The transmitter first reads in the binary file it wants to send,
and then sends the bytes to the Raspberry Pi as a series of packets.
This is the algorithm used in the transmitter.

1 Start the transmission by sending the SOH character,
which has the value 0x01.
SOH is a *control character* which stands for *start of transmission*.

2 Next send the packet number as a byte.
The first packet is numbered 1,
and the packet number is incremented after each packet is sent.

3 Next send a byte 
whose value is the complement of the packet number,
or `~packet_number`.

4 Send a 128 byte chunk of the binary file.

5 Finally, send a checksum byte.

6 Repeat the above for all the packets being transmitted.
When there are no more packets to be sent,
send the EOT character; EOT stands for *end of transmission*.

Now let's look at the algorithm used to receive the XMODEM
protocol, as implemented in the file `bootloader.c`.
This program, which runs on the Raspberry Pi, is the receiver.
It reads bytes using the UART.
The receiver does the following:

1 Wait for a SOH.

1 Read the packet number. 
The first block must be packet number 1.
The packet number of each packet should be incremented
after each packet is successfully received.
If the receiver sees a packet with the wrong packet number,
the receiver should send the control character `NAK`, for 
negative acknowledge, to the transmitter.

2 Check that the complement of the packet number is correct.
How does the C code in bootloader check that the complement is correct?
If the packet number and 
the complement of the packet number are not consistent, 
send a `NAK` to the transmitter.

3 Read the 128 bytes comprising the payload of the packet.
Incrementally update the cyclic redundancy check (CRC) when a byte arrives.
The CRC is formed by adding together mod 256 all the bytes in the packet.
How is this done in the bootloader?
Suppose we send 128 bytes,
where the 1st byte is 1, the 2nd byte is 2, and so on, until
we get to 128th byte which has the value 128.
What is the value of the CRC in this case?

4 After all 128 bytes have been sent,
read the CRC byte sent by the transmitter.
Compare the CRC sent by the transmitter 
with the calculated CRC.
If they agree, send an `ACK` (acknowledge) to the transmitter;
if they do not agree, send a `NAK` (not acknowledge) to the transmitter.

5 If the packet has been successfully received,
copy it to memory,
and prepare to receive the next packet.

6 If the receiver receives the EOT byte,
then the tranmission has ended.
EOT stands for *end of transmission*.
Send an `ACK`,
and then jump to the program memory location.

Read `bootloader.c` and figure out how it works.
Where does the bootloader copy the program?
What happens when the bootloader detects an error
which requires it to send a `NAK`?
How does it recover from errors?
Why does it use the timer?

Discuss the bootloader code in detail with your lab mates.
Make sure you understand all of it.

Each group is responsible for
writing a description of the C implementation of the bootloader.
Break the description up into parts,
one for each person in your group.
Assign part of the description to each person.
Each person should write one paragraph 
describing the part of the implementation assigned to them.
Collate your descriptions, and hand in the completed writeup to the CA.


