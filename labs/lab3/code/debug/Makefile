ARMGNU ?= arm-none-eabi

CC = $(ARMGNU)-gcc
LD = $(ARMGNU)-ld
AS = $(ARMGNU)-as
OBJCOPY = $(ARMGNU)-objcopy
OBJDUMP = $(ARMGNU)-objdump

CPPFLAGS = -I../include
CFLAGS = -std=c99 -Wall -Og -g -ffreestanding $(CPPFLAGS)
ASFLAGS = 
LDFLAGS  = -nostdlib -T memmap -L../lib 
LDLIBS = -lpi

.SUFFIXES: 

NAME = program
SRCS = $(NAME).c

all : $(NAME).bin

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< 
	$(CC) -MM $(CFLAGS) -c $< > $*.d

%.exe: start.o %.o
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@ 

%.bin: %.exe
	$(OBJCOPY) $< -O binary $@

%.list: %.o
	$(OBJDUMP) -d --no-show-raw-insn $< > $@

clean :
	rm -f *.bin
	rm -f *.exe
	rm -f *.o
	rm -f *.d
	rm -f *.list

install: $(NAME).bin
	rpi-install.py $(NAME).bin

# Use the system compiler for your (OS X or Linux) computer.
local: $(NAME).c
	cc -std=c99 -Wall -O2 -DLOCAL_TEST -o $(NAME) -g $(NAME).c

run-local: local
	./$(NAME)

.PHONY : all clean

.PRECIOUS: %.exe

# include dependency info 
#  "-" suppresses errors
-include $(SRCS:.c=.d)
