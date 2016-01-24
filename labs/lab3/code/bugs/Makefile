ARMGNU = arm-none-eabi
CFLAGS  = -O0 -g -Wall -nostdlib -nostartfiles -ffreestanding 
CC = $(ARMGNU)-gcc

NAME = warn

all:
	$(ARMGNU)-gcc $(CFLAGS) start.s $(NAME).c -o $(NAME)
	$(ARMGNU)-objdump $(NAME) -d > $(NAME).list

clean:
	rm -f $(NAME) *.list  *.o *~



